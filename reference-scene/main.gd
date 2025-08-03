extends Node3D

@onready var cam = $Camera3D
var camera_rotation = Vector2(0.4, 0.4)
var distance = 8.0

func _input(event):
	if event is InputEventMouseMotion and Input.is_mouse_button_pressed(MOUSE_BUTTON_RIGHT):
		camera_rotation.x -= event.relative.y * 0.01
		camera_rotation.y -= event.relative.x * 0.01
		camera_rotation.x = clamp(camera_rotation.x, -1.5, 1.5)

func _process(delta):
	var offset = Vector3(
		distance * cos(camera_rotation.x) * sin(camera_rotation.y),
		distance * sin(camera_rotation.x),
		distance * cos(camera_rotation.x) * cos(camera_rotation.y)
	)
	cam.global_position = global_position + offset
	cam.look_at(global_position)
