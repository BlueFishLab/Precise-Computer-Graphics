extends Node3D

@onready var camera = $Camera3D
@onready var button = $CanvasLayer/Control/HBoxContainer/StartSketchButton

var transition_time = 0.5  # czas animacji


func _ready():
	button.pressed.connect(_on_button_pressed)
	
	
func _on_button_pressed():
	# Wyłącz sterowanie kamerą
	camera.movement_enabled = false  # ⛔ zatrzymaj kamerę

	var tween = get_tree().create_tween()

	var target_position = Vector3(0, 5, 0)  # Nad środkiem
	var target_rotation = Vector3(-deg_to_rad(90), 0, 0)  # Patrzy w dół

	tween.tween_property(camera, "global_position", target_position, transition_time)
	tween.tween_property(camera, "rotation", target_rotation, transition_time)

	tween.tween_callback(func():
		print("Kamera zablokowana.")
	)
	
