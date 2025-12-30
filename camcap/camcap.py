import cv2
from flask import Flask, Response, jsonify
from flask_cors import CORS
import os 
import base64
import threading
import dashscope

app = Flask(__name__)
CORS(app)
cap = cv2.VideoCapture(0)
cap_lock = threading.Lock() 

QWEN_API_KEY = os.getenv('QWEN_API_KEY')

@app.route("/snapshot_desc")
def snapshot_desc():
    with cap_lock:
        ret, frame = cap.read()

    if not ret:
        return jsonify({"error": "camera read failed"}), 500

    # OpenCV → JPEG → base64
    _, jpeg = cv2.imencode(".jpg", frame)
    img_b64 = base64.b64encode(jpeg).decode()

    messages = [
        {
            "role": "user",
            "content": [
                {
                    "image": f"data:image/jpeg;base64,{img_b64}"
                },
                {
                    "text": "请用一句简洁的话描述画面内容"
                }
            ]
        }
    ]

    response = dashscope.MultiModalConversation.call(
        api_key=os.getenv("QWEN_API_KEY"),
        model="qwen3-vl-flash",
        messages=messages
    )

    # 健壮一点，防止结构变化
    try:
        text = response.output.choices[0].message.content[0]["text"]
    except Exception:
        return jsonify({"error": "invalid qwen response"}), 500

    return jsonify({
        "description": text
    })

def gen():
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        _, jpeg = cv2.imencode('.jpg', frame)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' +
               jpeg.tobytes() + b'\r\n')

@app.route('/video')
def video():
    return Response(gen(),
        mimetype='multipart/x-mixed-replace; boundary=frame')

app.run(host='0.0.0.0', port=5000)


