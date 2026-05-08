#pragma once
#include <string>

/** Independent helpers for default-card asset paths. */
class CardAssetUtils {
  public:
  /** Returns the common card background path. */
  static std::string cardBackgroundPath();

  /** Returns the rank label used in number asset names. */
  static std::string rankNameForFace(int face);

  /** Returns red or black for a suit value. */
  static std::string colorNameForSuit(int suit);

  /** Returns the suit asset name for a suit value. */
  static std::string suitNameForSuit(int suit);

  /** Returns the big number asset path. */
  static std::string bigRankPath(int face, int suit);

  /** Returns the small number asset path. */
  static std::string smallRankPath(int face, int suit);

  /** Returns the suit icon asset path. */
  static std::string suitPath(int suit);
};
