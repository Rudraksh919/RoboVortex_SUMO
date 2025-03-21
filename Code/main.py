import pygame
import requests
import time

# ESP32 details
ESP32_IP = "192.168.4.1"  # Change if needed
COMMANDS = {
    "forward": "/forward",
    "backward": "/backward",
    "left": "/left",
    "right": "/right",
    "stop": "/stop",
    "motor_cw": "/motorcw",
    "motor_ccw": "/motorccw",
    "motor_stop": "/motorstop"
}

# Initialize pygame and joystick
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Detected Joystick: {joystick.get_name()}")

def send_command(command):
    url = f"http://{ESP32_IP}{COMMANDS[command]}"
    try:
        requests.get(url, timeout=0.5)
        print(f"Sent command: {command}")
    except requests.exceptions.RequestException:
        print(f"Failed to send {command}")

last_drive_command = ""
last_motorC_command = ""

while True:
    pygame.event.pump()  # Process events
    
    # Movement control (axis based)
    axis_0 = joystick.get_axis(0)
    axis_1 = joystick.get_axis(1)
    
    if axis_1 < -0.5:
        if last_drive_command != "forward":
            send_command("forward")
            last_drive_command = "forward"
    elif axis_1 > 0.5:
        if last_drive_command != "backward":
            send_command("backward")
            last_drive_command = "backward"
    elif axis_0 < -0.5:
        if last_drive_command != "left":
            send_command("left")
            last_drive_command = "left"
    elif axis_0 > 0.5:
        if last_drive_command != "right":
            send_command("right")
            last_drive_command = "right"
    else:
        if last_drive_command != "stop":
            send_command("stop")
            last_drive_command = "stop"

    # Motor C control (button based)
    button_cw = joystick.get_button(0)
    button_ccw = joystick.get_button(1)

    if button_cw:
        if last_motorC_command != "motor_cw":
            send_command("motor_cw")
            last_motorC_command = "motor_cw"
    elif button_ccw:
        if last_motorC_command != "motor_ccw":
            send_command("motor_ccw")
            last_motorC_command = "motor_ccw"
    else:
        if last_motorC_command not in ["", "motor_stop"]:
            send_command("motor_stop")
            last_motorC_command = "motor_stop"

    time.sleep(0.05)  # Control request frequency
