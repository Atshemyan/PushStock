#include <random>

class Coin
{
public:
    virtual void editPrice() = 0;
    long double getPrice() const;
    virtual std::string getName() const = 0;
protected:
    long double m_price {};
    std::random_device m_rd;
};

class Vhuk : public Coin
{
public:
    void editPrice() override;
    std::string getName() const override;
};

class Hke : public Coin
{
public:
    void editPrice() override;
    std::string getName() const override;
};

class NarSmoke : public Coin
{
public:
    void editPrice() override;
    std::string getName() const override;
};

class Voyl : public Coin
{
public:
    void editPrice() override;
    std::string getName() const override;
};