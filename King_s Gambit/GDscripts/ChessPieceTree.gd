extends Tree


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


func _ready():
	
	var gameSettings = create_item()
	gameSettings.set_text(0, "GameSettings")
	
	hide_root = true
	
	var chessPieceAmounts = create_item(gameSettings)
	chessPieceAmounts.set_text(0, "ChessPieceAmounts")
	
	var pawnAmount = create_item(chessPieceAmounts)
	pawnAmount.set_text(0, "Pawn")
	
	var knightAmount = create_item(chessPieceAmounts)
	knightAmount.set_text(0, "Knight")
	
	var bishopAmount = create_item(chessPieceAmounts)
	bishopAmount.set_text(0, "Bishop")



