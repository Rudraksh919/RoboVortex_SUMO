import websocket
import pygame
import time

# ESP32 WebSocket Server IP
ESP_IP = "192.168.4.1"
ESP_PORT = 81
WS_URL = f"ws://{ESP_IP}:{ESP_PORT}/"

# Connect to ESP32 WebSocket Server
ws = websocket.WebSocket()
ws.connect(WS_URL)

# Initialize Pygame for Controller Input
pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

DEAD_ZONE = 0.2  # Ignore small movements
last_command = ""  # Track last sent command

while True:
    pygame.event.pump()
    
    axis_y = joystick.get_axis(1)  # Forward/Backward
    axis_x = joystick.get_axis(0)  # Left/Right

    # Determine movement command
    if axis_y < -DEAD_ZONE:
        command = "forward"
    elif axis_y > DEAD_ZONE:
        command = "backward"
    elif axis_x < -DEAD_ZONE:
        command = "left"
    elif axis_x > DEAD_ZONE:
        command = "right"
    else:
        command = "stop"

    # Send command only if it's different from the last one
    if command != last_command:
        ws.send(command)
        last_command = command  # Update last command
    
    time.sleep(0.05)  # Small delay to prevent spamming commands