from flask import Flask, render_template
import RPi.GPIO as GPIO
import threading
import time
import subprocess
import socket


app = Flask(__name__)

PIR_PIN = 21
buzzer_pin = 26


button_pins = [25,10,17,18]

GPIO.setmode(GPIO.BCM)

for button_pin in button_pins:
    GPIO.setup(button_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.setup(PIR_PIN, GPIO.IN)

GPIO.setup(27, GPIO.OUT)
GPIO.setup(buzzer_pin, GPIO.OUT)

motion_detected = False

def buzz(frequency, duration):
    period = 1.0 / frequency
    cycles = int(duration * frequency)
    for i in range(cycles):
        GPIO.output(buzzer_pin, GPIO.HIGH)
        time.sleep(period / 2)
        GPIO.output(buzzer_pin, GPIO.LOW)
        time.sleep(period / 2)

    
# Funkcja do wprowadzenia hasła za pomocą przycisków
def input_password():
    password = ""
    while True:
        for button_pin in button_pins:
            if GPIO.input(button_pin) == GPIO.LOW:
                if button_pin == 25:
                    password += "1"
                elif button_pin == 10:
                    password += "2"
                elif button_pin == 17:
                    password += "3"
                elif button_pin == 18:
                    password += "4"
                time.sleep(0.2)  # Opóźnienie, aby uniknąć wielokrotnego naciśnięcia tego samego przycisku
        if len(password) == 4:
            return password

# Funkcja do uruchomienia alarmu
def trigger_alarm():
    # Implementacja aktywacji alarmu
    buzz(1000, 0.5)  # 1000 Hz przez 0.5 sekundy
    time.sleep(0.5)
    buzz(1500, 0.5)  # 1500 Hz przez 0.5 sekundy
    time.sleep(0.5)
    buzz(2000, 0.5)  # 




def motion_detected_callback():
    print("Podaj haslo!")
    password = input_password()
    # Poczekaj na wprowadzenie hasła
    if password != "1234":  # Przykładowe hasło
        print("Incorrect password! Initiating alarm...")
        trigger_alarm()
        send_to_netcat()



def send_to_netcat():
    print("Sending alarm to server")
    # Komenda do wysłania wiadomości za pomocą Netcat
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('192.168.145.101', 12345))
    client_socket.sendall(b'Alarm activated! Motion detected. Possible intruder.')
    client_socket.close()



def monitor_motion():
    global motion_detected
    while True:
        if GPIO.input(PIR_PIN):
            print("wykryto ruch")
            motion_detected = True
            motion_detected_callback()
            
        else:
            print("nie wykryto ruchu")
            motion_detected = False
            
        time.sleep(1)

@app.route('/')
def index():
    return render_template('indexLab4.html', motion=motion_detected)


if __name__ == '__main__':


   


    t = threading.Thread(target=monitor_motion)
    t.start()
    app.run(host='0.0.0.0', port=5000)

        # Czyszczenie ustawień GPIO
    GPIO.cleanup()
