extends Node2D

var paused = false;

func _ready():
	pause_mode = Node.PAUSE_MODE_PROCESS
		
func _input(event):
	if event is InputEventKey and event.pressed:
		if event.scancode == KEY_P:
			if Input.is_key_pressed(KEY_P) and paused == false:
				paused = true
				get_tree().paused = paused
			elif Input.is_key_pressed(KEY_P) and paused == true:
				paused = false
				get_tree().paused = paused
