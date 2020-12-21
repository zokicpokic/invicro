import os, io, logging, json
from flask import Flask, request
from flask_cors import CORS, cross_origin

app = Flask(__name__)

cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'


@app.route("/") 
def index():
    return "Token service.<br>GET /validate<br>POST /create"

@app.route("/validate/<path:token>/", methods=['GET'])
@cross_origin()
def validate(token):
    return json.loads(json.dumps(
        {
            "token_ok": token
        }))

@app.route("/create/<path:token>/", methods=['POST'])
@cross_origin()
def create(token):
    return json.loads(json.dumps(
        {
            "create_token": token
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
