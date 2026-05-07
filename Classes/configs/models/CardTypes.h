#pragma once

// Suit values used by the level config.
enum CardSuitType {
  CST_NONE = -1,
  CST_CLUBS,
  CST_DIAMONDS,
  CST_HEARTS,
  CST_SPADES,
  CST_NUM_CARD_SUIT_TYPES
};

// Face values used by the level config.
enum CardFaceType {
  CFT_NONE = -1,
  CFT_ACE,
  CFT_TWO,
  CFT_THREE,
  CFT_FOUR,
  CFT_FIVE,
  CFT_SIX,
  CFT_SEVEN,
  CFT_EIGHT,
  CFT_NINE,
  CFT_TEN,
  CFT_JACK,
  CFT_QUEEN,
  CFT_KING,
  CFT_NUM_CARD_FACE_TYPES
};

// Runtime area occupied by a card.
enum CardAreaType {
  CAT_NONE = -1,
  CAT_PLAYFIELD,
  CAT_STACK,
  CAT_TRAY
};
