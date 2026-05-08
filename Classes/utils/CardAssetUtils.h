#pragma once
#include <string>

/**
 * @brief Builds resource paths and asset-name fragments for default cards.
 */
class CardAssetUtils {
  public:
  /**
   * @brief Returns the common card background path.
   *
   * @return Resource path for the shared card background image.
   */
  static std::string cardBackgroundPath();

  /**
   * @brief Returns the rank label used in number asset names.
   *
   * @param face Face value where 0 is ace and 12 is king.
   * @return Rank label used inside asset filenames.
   */
  static std::string rankNameForFace(int face);

  /**
   * @brief Returns red or black for a suit value.
   *
   * @param suit Suit value where 0 is club, 1 is diamond, 2 is heart, and 3 is spade.
   * @return Color name used inside rank asset filenames.
   */
  static std::string colorNameForSuit(int suit);

  /**
   * @brief Returns the suit asset name for a suit value.
   *
   * @param suit Suit value where 0 is club, 1 is diamond, 2 is heart, and 3 is spade.
   * @return Suit name used inside suit asset filenames.
   */
  static std::string suitNameForSuit(int suit);

  /**
   * @brief Returns the big rank image asset path.
   *
   * @param face Face value where 0 is ace and 12 is king.
   * @param suit Suit value used to choose red or black rank art.
   * @return Resource path for the large rank image.
   */
  static std::string bigRankPath(int face, int suit);

  /**
   * @brief Returns the small rank image asset path.
   *
   * @param face Face value where 0 is ace and 12 is king.
   * @param suit Suit value used to choose red or black rank art.
   * @return Resource path for the small rank image.
   */
  static std::string smallRankPath(int face, int suit);

  /**
   * @brief Returns the suit icon asset path.
   *
   * @param suit Suit value where 0 is club, 1 is diamond, 2 is heart, and 3 is spade.
   * @return Resource path for the suit icon image.
   */
  static std::string suitPath(int suit);
};
