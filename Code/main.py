import pygame
import requests
import time

# ESP32 details
ESP32_IP = "192.168.4.1"  # Make sure this matches the ESP32 AP IP
COMMANDS = {
    "forward": "/forward",
    "backward": "/backward",
    "left": "/left",
    "right": "/right",
    "stop": "/stop",
    "motor_cw": "/motorcw",       # NEW: Motor C Clockwise
    "motor_ccw": "/motorccw"      # NEW: Motor C    AntiClockwise

}

# Initialize pygame and joystick
pygame.init()
pygame.joystick.init()

# Detect joystick
if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    exit()
joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Detected Joystick: {joystick.get_name()}")

def send_command(command):
    url = f"http://{ESP32_IP}{COMMANDS[command]}"
    try:
        requests.get(url, timeout=1)
        print(f"Sent command: {command}")
    except requests.exceptions.RequestException:
        print(f"Failed to send {command}")

# Main loop
last_command = ""
while True:
    pygame.event.pump()  # Process internal pygame events
    
    # Example: using left joystick (axis 1 for vertical, axis 0 for horizontal)
    axis_0 = joystick.get_axis(0)
    axis_1 = joystick.get_axis(1)
    
    # Determine direction based on axis values
    if axis_1 < -0.5:
        if last_command != "forward":
            send_command("forward")
            last_command = "forward"
    elif axis_1 > 0.5:
        if last_command != "backward":
            send_command("backward")
            last_command = "backward"
    elif axis_0 < -0.5:
        if last_command != "left":
            send_command("left")
            last_command = "left"
    elif axis_0 > 0.5:
        if last_command != "right":
            send_command("right")
            last_command = "right"
    else:
        if last_command != "stop":
            send_command("stop")
            last_command = "stop"

    
    button_cw = joystick.get_button(0)
    button_ccw = joystick.get_button(1)
    
   if button_cw:
        if last_command != "motor_cw":
            
            send_command("motor_cw")
            last_command = "motor_cw"
            
    elif button_ccw:
        if last_command != "motor_ccw":
            send_command("motor_ccw")
            last_command = "motor_ccw"

    time.sleep(0.1)  # Prevent spamming requests too fast
     