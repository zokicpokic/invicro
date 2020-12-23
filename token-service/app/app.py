import os, io, logging, json
from flask import Flask, request, make_response
from flask_cors import CORS, cross_origin
import time
import base64

app = Flask(__name__)

cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

TOKEN_DURATION = 10 * 60; # token duration in seconds (10 minutes)


@app.route("/") 
def index():
    return "Token service.<br>GET /validate<br>POST /create"

@app.route("/validate/<path:token>", methods=['GET'])
@cross_origin()
def validate(token):
    try:
        token_decoded = base64.b64decode(token)
        token = float(token_decoded)
        now = time.time()

        delta = token - now

        if delta > 0:
            return json.loads(json.dumps(
                {
                    "ok": token
                }))
        else:
            return make_response('Token has expired', 403)
    except:
        return make_response('Error parsing token', 400)



@app.route("/create", methods=['POST'])
@cross_origin()
def create():
    now = time.time(); # seconds since 1970
    exp_time = now + TOKEN_DURATION

    token_bytes = str(exp_time).encode('ascii')
    token_base64 = base64.b64encode(token_bytes)

    return json.loads(json.dumps(
        {
            "token": token_base64.decode('ascii')
        }))

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
