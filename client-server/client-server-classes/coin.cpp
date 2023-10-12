#include "coin.hpp"

long double Coin::getPrice() const
{
    return m_price;
}

void Vhuk::editPrice()
{
    std::mt19937 generator(m_rd());
    std::uniform_real_distribution<double> distribution(-100.0, 100.0);
    if (m_price + distribution(generator) >= 1800)
    {
        m_price -= 500;
        return;
    }
    else if (m_price + distribution(generator) < 200)
    {
        m_price += 500;
        return;
    }
    m_price += distribution(generator);
}

std::string Vhuk::getName() const 
{
    return "VHUK";
}

void Hke::editPrice()
{
    std::mt19937 generator(m_rd());
    std::uniform_real_distribution<double> distribution(-100.0, 100.0);
   if (m_price + distribution(generator) >= 1800)
    {
        m_price -= 500;
        return;
    }
    else if (m_price + distribution(generator) <= 200)
    {
        m_price += 500;
        return;
    }
    m_price += distribution(generator);
}

std::string Hke::getName() const
{
    return "HKE";
}

void NarSmoke::editPrice()
{
    std::mt19937 generator(m_rd());
    std::uniform_real_distribution<double> distribution(-100.0, 100.0);
    if (m_price + distribution(generator) >= 1800)
    {
        m_price -= 500;
        return;
    }
    else if (m_price + distribution(generator) < 200)
    {
        m_price += 500;
        return;
    }
    m_price += distribution(generator);
}

std::string NarSmoke::getName() const
{
    return "NARSMOKE";
}

void Voyl::editPrice()
{
    std::mt19937 generator(m_rd());
    std::uniform_real_distribution<double> distribution(-100.0, 100.0);
    if (m_price + distribution(generator) >= 1800)
    {
        m_price -= 500;
        return;
    }
    else if (m_price + distribution(generator) < 200)
    {
        m_price += 500;
        return;
    }
    m_price += distribution(generator);
}

std::string Voyl::getName() const
{
    return "VOYL";
}
