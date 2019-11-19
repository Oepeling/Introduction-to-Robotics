const int noOfPins = 8;
const int sigma = 10;

byte displayEncoding[sigma][noOfPins] = {
  //  a  b  c  d  e  f  g
  {0, 1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 0, 1, 1, 0, 0, 0, 0}, // 1
  {0, 1, 1, 0, 1, 1, 0, 1}, // 2
  {0, 1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 0, 1, 1, 0, 0, 1, 1}, // 4
  {0, 1, 0, 1, 1, 0, 1, 1}, // 5
  {0, 1, 0, 1, 1, 1, 1, 1}, // 6
  {0, 1, 1, 1, 0, 0, 0, 0}, // 7
  {0, 1, 1, 1, 1, 1, 1, 1}, // 8
  {0, 1, 1, 1, 1, 0, 1, 1}  // 9
};