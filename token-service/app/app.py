import os, io, logging, json
from flask import Flask, request, make_response
from flask_cors import CORS, cross_origin
import time
import base64
import requests

app = Flask(__name__)

cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

TOKEN_DURATION = os.getenv('TOKEN_DURATION')
if TOKEN_DURATION == None:
    TOKEN_DURATION = 10 * 60; # token duration in seconds (10 minutes)
else:
    TOKEN_DURATION = int(TOKEN_DURATION)


VAULT_ADDR = os.getenv('VAULT_API_ADDR');
if VAULT_ADDR == None:
    VAULT_ADDR = 'http://localhost:8200/';

print('TOKEN_DURATION ' + str(TOKEN_DURATION))
print('VAULT_API_ADDR ' + VAULT_ADDR);
@app.route("/") 
def index():
    return "Token service.<br>GET /validate<br>POST /create"

@app.route("/validate", methods=['GET'])
@cross_origin()
def validate():
    try:
        token = request.args.get('token');
        auth = request.args.get('auth_token');
        print(token);
        print(auth);

        token_decoded = base64.b64decode(token)

        vaultEncryptUrl = VAULT_ADDR + 'v1/pathology/decrypt/viewer';
        resp = requests.post(vaultEncryptUrl,
                             data= {'ciphertext':token_decoded.decode('ascii')},
                             headers = {
                                 'X-Vault-Token': auth
                             })

        print(resp.status_code);

        if resp.status_code == 200:
            response_json = json.loads(resp.text);
            plaintext_base64 = response_json["data"]["plaintext"];

            token_text = base64.b64decode(plaintext_base64)

            token_json = json.loads(token_text);

            exp_time = float(token_json["exp_time"]);
            delta = exp_time - time.time();

            if delta > 0:
                return token_json;
            else:
                return make_response('Token has expired', 403)
        else:
            return make_response(resp.text, resp.status_code);

    except:
        return make_response('Error parsing token', 400)

@app.route("/create", methods=['POST'])
@cross_origin()
def create():
    now = time.time(); # seconds since 1970
    exp_time = now + TOKEN_DURATION;

    auth_token = request.form.get('auth_token');
    print(auth_token);
    if auth_token == None:
        return make_response('Invalid or no auth_token', 400);

    data = {
        'exp_time': exp_time,
        'some_data': 'abc123'
    };

    dataJson = json.dumps(data);
    data_bytes = dataJson.encode('ascii');
    data_base64 = base64.b64encode(data_bytes);
    print(data_base64);
    vaultEncryptUrl = VAULT_ADDR + 'v1/pathology/encrypt/viewer';
    print(vaultEncryptUrl);

    resp = requests.post(vaultEncryptUrl,
                         data= {'plaintext':data_base64.decode('ascii')},
                  headers = {
                      'X-Vault-Token': auth_token
                  })

    print('vault resp');
    print(resp.status_code);

    if resp.status_code == 200:
        print('ok');
        response_json = json.loads(resp.text);
        cipher = response_json["data"]["ciphertext"];
        print(cipher);

        token_bytes = cipher.encode('ascii')
        token_base64 = base64.b64encode(token_bytes)

        return json.loads(json.dumps(
            {
                "token": token_base64.decode('ascii')
            }))
    else:
        print('not ok');
        return make_response(resp.text, resp.status_code);

# Set log configuration
logging.basicConfig(filename='flask.log',
                    filemode='a',
                    format='%(asctime)s,%(msecs)d %(name)s %(levelname)s %(message)s',
                    datefmt='%H:%M:%S',
                    level=logging.INFO)

# Run flask application
print("Starting token service...")
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5001, debug=True)
