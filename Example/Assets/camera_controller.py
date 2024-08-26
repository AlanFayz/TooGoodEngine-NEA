import TooGoodEngine 
import math  #python math
import Entity 
from Input import KeyCode
from CameraControllers import PerspectiveCameraController
from Math import Vector, normalize, cross #engine Math with vector class and utility

camera_controller = None 
camera_speed = 10.0

camera_position = Vector((-6.0, 2.0, -1.7))
camera_front    = Vector((6.0, 0.47, 1.59))
camera_up       = Vector((0.0, -1.0, 0.0))

last_x = 0
last_y = 0

yaw = 0
pitch = 0

def on_create():
    global camera_controller 

    camera_entity = Entity.get_entity_by_name("Camera")
    camera_controller = PerspectiveCameraController(camera_entity)

    pass 

def on_update(delta):
    global camera_controller
    global camera_position
    global camera_front
    global camera_up
    global camera_speed
    global last_x 
    global last_y
    global yaw 
    global pitch 

    w_key_down = TooGoodEngine.is_key_down(KeyCode.W);
    s_key_down = TooGoodEngine.is_key_down(KeyCode.S);
    a_key_down = TooGoodEngine.is_key_down(KeyCode.A);
    d_key_down = TooGoodEngine.is_key_down(KeyCode.D);
    q_key_down = TooGoodEngine.is_key_down(KeyCode.Q);
    e_key_down = TooGoodEngine.is_key_down(KeyCode.E);

    current_x, current_y = TooGoodEngine.get_mouse_coordinates()

    x_difference = current_x - last_x 
    y_difference = current_y - last_y

    last_x = current_x
    last_y = current_y

    movement = Vector([0.0, 0.0, 0.0])

    if w_key_down:
        movement -= camera_up * camera_speed * delta
    
    if s_key_down:
        movement += camera_up * camera_speed * delta
    
    side = normalize(cross(camera_front, camera_up))

    if a_key_down:
        movement += side * camera_speed * delta
    
    if d_key_down:
        movement -= side * camera_speed * delta
    
    if q_key_down:
        movement -= camera_front * camera_speed * delta
    
    if e_key_down:
        movement += camera_front * camera_speed * delta

    camera_position += movement 
    camera_controller.update_position(camera_position)
    
    yaw += x_difference
    pitch -= y_difference 

    pitch = max(pitch, -89.0)
    pitch = min(pitch, 89.0)

    direction = Vector([0.0, 0.0, 0.0])
    direction[0] = math.cos(math.radians(pitch)) * math.cos(math.radians(yaw))
    direction[1] = math.sin(math.radians(pitch))
    direction[2] = math.cos(math.radians(pitch)) * math.sin(math.radians(yaw))

    camera_front = normalize(direction)
    camera_controller.update_front(camera_front)

    pass