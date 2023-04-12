extends TextureButton

onready var pressAudio: AudioStreamPlayer2D = $"/root/MainScene/RestartButton/pressAudio"

func _on_RestartButton_pressed():
	#pressAudio.play();
	get_tree().reload_current_scene()
