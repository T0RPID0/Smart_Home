from flask import Flask,render_template,url_for,abort,request,redirect

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('test.html')

@app.route('/todo/api/v1.0/tasks', methods=['POST'])
def testing():
    if not request.json:
        abort(400)
#    print(task)
    return request.json

if __name__=="__main__":
    app.run(host='0.0.0.0',port=5000)