#pragma once
#include <vector>

#include "card.hpp"
#include "common/currency.hpp"


namespace BlackJack{
class PlayerStateBase;

struct WinState {
  enum class Outcome { Win, Loss, Push } outcome;
  currency_t win_amount;
  std::string reason;
};

struct PlayerHand {
  void calc_value();
  void add_card(Card card);
  bool busted() const noexcept;
  bool blackjack() const noexcept { return m_blackjack; }
  void check_blackjack() noexcept;
  auto check_win_state(const PlayerHand &dealer_hand) -> WinState;

  std::vector<Card> cards = {};
  int value{0};
  int softness{0};
  currency_t current_bet{0};
  bool m_blackjack = false;
};

std::ostream &operator<<(std::ostream &os, const PlayerHand &ph);
}