extends Camera3D

@export var speed := 5.0
var rotation_x := 0.0
var rotation_y := 0.0

var movement_enabled := true  # ✅ Flaga blokująca ruch

func _process(delta):
	if not movement_enabled:
		return  # ⛔ zablokuj cały ruch

	var dir := Vector3.ZERO
	if Input.is_action_pressed("move_forward"):
		dir -= transform.basis.z
	if Input.is_action_pressed("move_back"):
		dir += transform.basis.z
	if Input.is_action_pressed("move_left"):
		dir -= transform.basis.x
	if Input.is_action_pressed("move_right"):
		dir += transform.basis.x

	dir = dir.normalized()
	translate(dir * speed * delta)
