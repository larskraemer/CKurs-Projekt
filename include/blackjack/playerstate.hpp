#pragma once
#include <memory>
#include <string>
#include <vector>
#include <string_view>

#include "common/currency.hpp"
#include "playerhand.hpp"
#include "Deck.hpp"
#include "playeraction.hpp"

namespace BlackJack
{
    class GameState;

    class PlayerStateBase
    {
    public:
        virtual void reset_round_state(const GameState &) = 0;
        virtual bool next_hand() noexcept = 0;

        virtual void split(Deck &) = 0;
        virtual auto double_down() -> currency_t = 0;

        [[nodiscard]] virtual auto current_hand() -> PlayerHand & = 0;
        [[nodiscard]] virtual auto current_hand() const -> const PlayerHand & = 0;
        [[nodiscard]] virtual auto name() const noexcept -> std::string_view = 0;
        [[nodiscard]] virtual auto money() const noexcept -> currency_t = 0;

        virtual void display_state(const GameState &) const = 0;

        virtual auto get_action(const GameState &) const -> PlayerAction = 0;
    };

    class PlayerState
        : public PlayerStateBase
    {
    public:
        PlayerState(std::string, currency_t);

        virtual void reset_round_state(const GameState &) override;
        virtual bool next_hand() noexcept override;
        void recv_money(currency_t amount) noexcept { m_money += amount; }

        virtual void split(Deck &) override;
        virtual auto double_down() -> currency_t override;

        [[nodiscard]] virtual auto current_hand() -> PlayerHand & override { return *m_hands.at(m_current_hand); }
        [[nodiscard]] virtual auto current_hand() const -> const PlayerHand & override { return *m_hands.at(m_current_hand); }
        [[nodiscard]] virtual auto name() const noexcept -> std::string_view override { return m_name; }
        [[nodiscard]] virtual auto money() const noexcept -> currency_t override { return m_money; }
        virtual void display_state(const GameState &) const override;
        virtual auto get_bet(currency_t min_bet) const -> currency_t;

        virtual auto get_action(const GameState &) const -> PlayerAction override;

        auto &hands() { return m_hands; }
        void add_hand(currency_t);

    private:
        std::string m_name;
        std::vector<std::unique_ptr<PlayerHand>> m_hands;
        size_t m_current_hand;
        currency_t m_money;
    };

    class DealerState
        : public PlayerStateBase
    {
    public:
        virtual void reset_round_state(const GameState &) override;
        virtual bool next_hand() noexcept override { return false; }

        virtual void split(Deck &) override {}
        virtual auto double_down() -> currency_t override { return currency_t{}; }

        [[nodiscard]] virtual auto current_hand() -> PlayerHand & override { return hand; }
        [[nodiscard]] virtual auto current_hand() const -> const PlayerHand & override { return hand; }
        [[nodiscard]] virtual auto name() const noexcept -> std::string_view override { return "DEALER"; };
        [[nodiscard]] virtual auto money() const noexcept -> currency_t override { return currency_t::max(); };
        virtual void display_state(const GameState &) const override;

        virtual auto get_action(const GameState &) const -> PlayerAction override;

    private:
        PlayerHand hand;
    };

    class AIPlayerState
        : public PlayerState
    {
    public:
        template<class... Ts>
        AIPlayerState(Ts... ts)
            : PlayerState(ts...)
        {}

        virtual auto get_bet(currency_t min_bet) const -> currency_t override;
        virtual auto get_action(const GameState &) const -> PlayerAction override;
    private:
    };
} // namespace BlackJack