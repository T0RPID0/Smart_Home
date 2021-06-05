from flask import Flask,render_template,url_for,abort,request,redirect,Response,send_file
import requests,json
import cv2
from motion import motion_detect
 
app = Flask(__name__)

#generate images for video_feed route
def gen_frames(camera):  
    while True:
        success, frame = camera.read()  # read the camera frame
        if not success:
            break
        else:
            encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 50]
            ret, buffer = cv2.imencode('.jpg', frame, encode_param)
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result


@app.errorhandler(404)
def not_found(e):
    return render_template("404.html") 

@app.route('/')
def index():
    return render_template('index.html')        

@app.route('/newdevice/<device>')
def newdevice(device=None):
    node_number,ip = device.split("&")
    node_number=int(node_number[-1])
    with open('devicesss.json','r') as devices:
        devices_json=json.load(devices)
        devices_json['nodes'][node_number]['ip']=ip+":301"
    with open('devicesss.json','w') as devices:
        json.dump(devices_json, devices)
    return "added"

@app.route('/api/v1/update',methods = ['GET','POST'])
def update():
    if request.method=='GET':
        with open('devicesss.json','r') as devices:
            return json.load(devices)
    if request.method == 'POST':
        if not request.json:
            abort(400)
        req_data=request.json
        #print(req_data)
        node_number=int(req_data['node'][-1])
        with open('devicesss.json','r') as devices:
            devices_json=json.load(devices)
            device=devices_json['nodes'][node_number]
        try:
            url=f"http://{device['ip']}/{req_data['pin']}/{req_data['status']}" 
            requests.get(url,timeout=3)
            device_status="reachable"
        except:
            device_status= "unreachable"
        if device_status=="reachable":
            devices_json['nodes'][node_number][req_data['pin']]=req_data['status']
        else:
            devices_json['nodes'][node_number][req_data['pin']]="off"
            req_data['status']=device_status
        with open('devicesss.json','w') as devices:
            json.dump(devices_json, devices)
        return req_data['status']


@app.route('/video_feed/<cam>')
def video_feed(cam):
    with open("devicesss.json","r") as devices:
        cc=json.load(devices)["cams"][int(cam[3:])-1]
    if cc['status']=='on':
        camera = cv2.VideoCapture(cc['ip'])
        return Response(motion_detect(camera), mimetype='multipart/x-mixed-replace; boundary=frame')
    else:
        print(f'no {cam} found')
        #return Response(url_for('static', filename='noimage.png'), mimetype='image/png; boundary=frame')
        return send_file('static\\noimage.png',mimetype='image/png')

if __name__=="__main__":
    app.run(host='0.0.0.0',port=5000)