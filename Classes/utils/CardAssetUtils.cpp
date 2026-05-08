#include "utils/CardAssetUtils.h"

std::string CardAssetUtils::cardBackgroundPath() {
  return "res/card_general.png";
}

std::string CardAssetUtils::rankNameForFace(int face) {
  static char const *ranks[] = {"A", "2", "3",  "4", "5", "6", "7",
                                "8", "9", "10", "J", "Q", "K"};

  if (face < 0 || face > 12) {
    return "A";
  }

  return ranks[face];
}

std::string CardAssetUtils::colorNameForSuit(int suit) {
  if (suit == 1 || suit == 2) {
    return "red";
  }

  return "black";
}

std::string CardAssetUtils::suitNameForSuit(int suit) {
  static char const *suits[] = {"club", "diamond", "heart", "spade"};

  if (suit < 0 || suit > 3) {
    return "club";
  }

  return suits[suit];
}

std::string CardAssetUtils::bigRankPath(int face, int suit) {
  return "res/number/big_" + colorNameForSuit(suit) + "_" +
         rankNameForFace(face) + ".png";
}

std::string CardAssetUtils::smallRankPath(int face, int suit) {
  return "res/number/small_" + colorNameForSuit(suit) + "_" +
         rankNameForFace(face) + ".png";
}

std::string CardAssetUtils::suitPath(int suit) {
  return "res/suits/" + suitNameForSuit(suit) + ".png";
}
