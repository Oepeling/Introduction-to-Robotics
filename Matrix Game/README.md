Game Name: Perilous Path

Description:
  Although this game probably doesn't sound familiar to all of you, you will find that it is as simple and easy to play as a classic game like Tetris or Snake.
  It transforms the well-known maze game into a short-term memory game, making it more challenging and exciting.

Game play:
  The point of the game is to connect two given dots without hitting a bomb.
  The bombs configuration will be shown to you for about 5 seconds at the beginning of the game, therefore when you make the path you will have to figure out the bad cells from your memory.

Functionality:
  The LCD display menu allows you to start a new game, quit the game you are playing, see the current high score and change some settings like the initial difficulty, the name of the player.
  Also, during playtime, the LCD display shows the level, difficulty, number of lives remaining and score.
  This menu is fully controllable by joystick.

How to play:
  When entering a game you will start with the first level of the chosen difficulty.  After passing a few levels of that difficulty, the level of difficulty will be increased. You will start the game with 5 lives.
  At the beginning of each level you will be shown a bomb configuration for about 5 seconds (the number of bombs will depend on the current difficulty).
  After that, the bombs will disappear and you will be shown the start and destination cell and you have to make a path that doesn't cross any bomb from the start cell to the destination cell. It is guaranteed that both the start and the destination cell do not contain bombs and that such a path exists.
  You will create the path using the joystick. The current position on the matrix will be marked by a blinking LED while the rest of the path and the destination point will be marked by always-on LEDs.
  If, while drawing the path, you enter a cell that contains a bomb, the level will instantly stop and you will lose a life. If you have no lives left, you lose the game.
  The score you achieve will be determined by the length of the path (shorter is better) and by the difficulty of the level. You will receive a bonus if you manage to draw a path of minimal length.

Hardware components:
  - Arduino UNO
  - LCD Display
  - 8x8 LED Matrix
  - Joystick
  - Game On / Off Switch
  - Power On / Off Switch
  - Potentiometer for setting LCD contrast
