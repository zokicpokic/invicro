import os, io, logging, json
from flask import Flask, request, send_file, send_from_directory, make_response
from flask_cors import CORS, cross_origin
import requests
import cv2, cppyy
import numpy as np

TOKEN_SERVICE= os.getenv('TOKEN_SERVICE');
if TOKEN_SERVICE == None:
    TOKEN_SERVICE = 'http://localhost:5001/';

print('TOKEN_SERVICE:');
print(TOKEN_SERVICE);

excluded_endpoints = ["/api/region"]

app = Flask(__name__)
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

# Define Cpp include/libs paths
app_path = '/app'
cppyy.add_include_path(os.path.join(app_path, 'dependencies', 'include'))
cppyy.add_include_path(os.path.join(app_path, 'libs', 'mld_loader', 'include'))
cppyy.add_include_path(os.path.join(app_path, 'libs', 'region_loader', 'include'))
cppyy.add_library_path(os.path.join(app_path, 'dependencies', 'lib'))
cppyy.add_library_path(os.path.join(app_path, 'libs', 'mld_loader', 'lib'))
cppyy.add_library_path(os.path.join(app_path, 'libs', 'region_loader', 'lib'))

# Import Cpp libraries methods
cppyy.include("mld_loader.h")
cppyy.include("region_loader.h")
cppyy.load_library("mld_loader")
cppyy.load_library("region_loader")
from cppyy.gbl import load_whole_mld, load_region, get_dimensions, free_memory, alloc_memory, load_thumbnail, alloc_memory_dim, free_memory_dim, get_thumbnail_dimensions, alloc_memory_v2, get_value

tile = 256

# serving frontend

@app.route("/") 
def index():
    print("requesting index.html");
    return send_file("static/index.html")

@app.route("/2")
def index2():
    print("requesting index.html");
    return send_file("static/index.html")

@app.route("/js/<path:path>")
def send_js(path):
    return send_file(os.path.join('static/js', path));

@app.route("/css/<path:path>")
def send_css(path):
    return send_file(os.path.join('static/css', path));

######################

# token validation middlevare

@app.before_request
def check_token():
    for ex in excluded_endpoints:
        if request.path.startswith(ex):
            print('Excluded endpoint')
            return;

    if request.method == 'OPTIONS':
        print('OPTIONS')
        return

    token = request.args.get('token')
    if token == None:
        token = ''

    auth_token = request.args.get('auth_token')
    if auth_token == None:
        auth_token = ''

    validateUrl = TOKEN_SERVICE + 'validate?token=' + token + '&auth_token=' + auth_token;
    print('validate url');
    print(validateUrl);
    response = requests.get(validateUrl)
    print(response.status_code);
    print(response.text);
    if response.status_code >= 400:
        print('token invalid')
        return make_response(response.text, response.status_code)


######################

def get_dir_items(path):
    d = {"name": os.path.basename(path)}
    d["id"] = path
    if os.path.isdir(path):
        d["type"] = "dir"
        d["items"] = [get_dir_items(os.path.join(path, x)) for x in os.listdir(path) if not x.lower().endswith((".mld", ".json"))]
    else:
        d["type"] = "file"
    return d

@app.route("/api/all/<path:directory>/", methods=['GET'])
@cross_origin()
def get_all(directory):
    directory = os.path.join(DATA_PATH, directory)

    print("directory: " + directory)

    files = [os.path.relpath(os.path.join(directory, f), DATA_PATH) for f in os.listdir(directory) if f.lower().endswith(".svs")]

    return json.loads(json.dumps(
        {
            "data": json.dumps(files)
        }))

@app.route("/api/all/", methods=['GET'])
@cross_origin()
def dir_tree():
    tree = get_dir_items(DATA_PATH)
    return json.dumps(tree)   

@app.route("/api/thumbnail/<path:filename>/", methods=['GET'])
@cross_origin()
def thumbnail(filename):
    filename = os.path.join("/", filename)

    print("thumbnail: " + filename)
    status = cppyy.gbl.std.string('')
    info = cppyy.gbl.std.string('')
    fname = cppyy.gbl.std.string(filename)

    width = alloc_memory_dim(info)
    height = alloc_memory_dim(info)

    status = get_thumbnail_dimensions(width, height, info, fname)
    if status == 0:
        data = alloc_memory_v2(info, width, height)
        info = cppyy.gbl.std.string('')

        status = load_thumbnail(data, info, fname)
        if status == 0:
            w = get_value(width)
            h = get_value(height)

            img = np.frombuffer(data, dtype=np.uint8, count=4*w*h).copy()
            img = np.reshape(img, (h, w, 4))

            status = free_memory(data)
            if status == 0:
                logging.info('THUMBNAIL REQUEST: "{0}"'.format(request.url))
                is_success, buffer = cv2.imencode(".png", img)
                io_buf = io.BytesIO(buffer)
                return send_file(io_buf, mimetype='image/png')
            else:
                logging.error('THUMBNAIL REQUEST: "{0}"'.format(request.url))
                logging.error('THUMBNAIL: Memory not freed.')
                return 'ERROR: Memory not freed.'
        else:
            status = free_memory(data)
            logging.error('THUMBNAIL REQUEST: "{0}"'.format(request.url))
            logging.error('THUMBNAIL - cannot load thumb: "{0}"'.format(info))
            return str(info)
    else:
        logging.error('THUMBNAIL REQUEST: "{0}"'.format(request.url))
        logging.error('THUMBNAIL - cannot get dimensions: "{0}"'.format(info))

    status = free_memory_dim(width)
    status = free_memory_dim(height)
    return str(info)


@app.route("/api/region/<path:filename>/<int:level>/<int:x>/<int:y>/", methods=['GET'])
@cross_origin()
def region(filename, level, x, y):
    filename = os.path.join(DATA_PATH, filename)

    print("region: " + filename)
    status = cppyy.gbl.std.string('')
    info = cppyy.gbl.std.string('')
    fname = cppyy.gbl.std.string(filename)

    data = alloc_memory(info, tile, tile)

    info = cppyy.gbl.std.string('')
    status = load_region(data, info, fname, tile, level, x, y)

    if status == 0:
        img = np.frombuffer(data, dtype=np.uint8, count=4*tile*tile).copy()
        img = np.reshape(img, (tile, tile, 4))

        status = free_memory(data)
        if status == 0:
            logging.info('REGION REQUEST: "{0}"'.format(request.url))
            logging.info('REGION: "{0}"'.format(info))
            logging.info('REGION SHAPE: {0}'.format(str(img.shape)))

            is_success, buffer = cv2.imencode(".png", img)
            io_buf = io.BytesIO(buffer)
            return send_file(io_buf, mimetype='image/png')
        else:
            logging.error('REGION REQUEST: "{0}"'.format(request.url))
            logging.error('REGION: Memory not freed.')
            return 'ERROR: Memory not freed.'
    else:
        logging.error('REGION REQUEST: "{0}"'.format(request.url))
        logging.error('REGION: "{0}"'.format(info))
        return str(info)


@app.route("/api/dimensions/<path:filename>/", methods=['GET'])
@cross_origin()
def dimensions(filename):
    filename = os.path.join(DATA_PATH, filename)

    data = cppyy.gbl.std.string('')
    info = cppyy.gbl.std.string('')
    fname = cppyy.gbl.std.string(filename)

    status = get_dimensions(data, info, fname)
    print(data)

    if status == 0:
        logging.info('DIMENSIONS REQUEST: "{0}"'.format(request.url))
        logging.info('DIMENSIONS: "{0}"'.format(info))
        return json.loads(str(data))
    else:
        logging.info('DIMENSIONS REQUEST: "{0}"'.format(request.url))
        logging.error('DIMENSIONS: "{0}"'.format(info))
        return str(info)

@app.route("/api/mld/<path:filename_img>/", methods=['GET'])
@cross_origin()
def mld_get(filename_img):
    filename_img = os.path.join(DATA_PATH, filename_img)
    filename_mld = os.path.join(''.join(filename_img.split('.')[:-1]), 'LayerData.mld')
    filename_json = os.path.join(''.join(filename_img.split('.')[:-1]), 'LayerData.json')

    print("--------------");
    print(filename_img);
    print(filename_mld);
    print(filename_json);
    data = cppyy.gbl.std.string('')
    info = cppyy.gbl.std.string('')

    if os.path.exists(filename_json):
        with open(filename_json, 'r') as json_file:
            data = json_file.read()
        return json.loads(json.dumps(
            {
                "type": "json",
                "data": json.loads(data)
            }))
    else:
        fname_img = cppyy.gbl.std.string(filename_img)
        fname_mld = cppyy.gbl.std.string(filename_mld)

        print("call load_whole_mld")
        status = load_whole_mld(data, info, fname_img, fname_mld)

        print(status)

        if status == 0:
            logging.info('MLD REQUEST: "{0}"'.format(request.url))
            logging.info('MLD: "{0}"'.format(info))

            # with open(filename_json, 'w') as json_file:
            #     json_file.write(str(data))
            return json.loads(json.dumps(
                {
                    "type": "mld",
                    "data": json.loads(str(data))
                }))
        else:
            logging.info('MLD REQUEST: "{0}"'.format(request.url))
            logging.error('MLD: "{0}"'.format(info))
            return str(info)


@app.route("/api/mld/<path:filename_img>/", methods=['POST'])
@cross_origin()
def mld_post(filename_img):
    filename_json = os.path.join(DATA_PATH, ''.join(filename_img.split('.')[:-1]), 'LayerData.json')
    data = request.get_json()

    success = False
    with open(filename_json, 'w') as json_file:
        json.dump(data, json_file)
        success = True

    logging.info('MLD REQUEST: "{0}"'.format(request.url))
    if success:
        return json.loads('{"detail":"success"}')

    logging.error('MLD: Could not write to a file "{0}"'.format(filename_json))
    return json.loads('{"detail":"failed"}')

# Set log configuration
logging.basicConfig(filename='flask.log',
                    filemode='a',
                    format='%(asctime)s,%(msecs)d %(name)s %(levelname)s %(message)s',
                    datefmt='%H:%M:%S',
                    level=logging.INFO)

# Set data location
DATA_PATH = "/data"
if "DATA_PATH" in os.environ:
    DATA_PATH = os.environ["DATA_PATH"]
print("DATA_PATH location: " + DATA_PATH)

# Run flask application
print("Starting server...")
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
