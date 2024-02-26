#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <ctime>

class Constants {
public:
    static const char currency_symbol = 163;
    static const int iterations = 10;
};

class Asset {
private:
    const std::string ticker;
    double price;
    int quantity;

public:
    // Constructor
    Asset(std::string ticker, double price, int quantity) : ticker(ticker) {
        this->price = price;
        this->quantity = quantity;
    }

    // Setters
    void setPrice(double price) {
        this->price = price;
    }
    void setQuantity(int quantity) {
        this->quantity = quantity;
    }

    // Getters
    double getPrice() const {
        return price;
    }
    std::string getTicker() const {
        return ticker;
    }
    int getQuantity() const {
        return quantity;
    }
};

// TODO: Add bond/stock derived classes to showcase polymorphism and inheritance

class Market {
private:
    std::vector<std::unique_ptr<Asset>> assets;

public:
    // Getters
    const std::vector<std::unique_ptr<Asset>>& getAssets() const {
        return assets;
    }

    // Get asset by ticker
    const Asset* getAssetByTicker(const std::string& ticker) const {
        for (const auto& asset : assets) {
            if (asset->getTicker() == ticker) {
                return asset.get();
            }
        }
        return nullptr;
    }

    // Add assets to market
    void addAsset(std::unique_ptr<Asset> asset) {
        assets.push_back(std::move(asset));
        std::sort(assets.begin(), assets.end(), [](const auto& a, const auto& b) {
            return a->getTicker() < b->getTicker();
        });
        //std::cout << "Asset added to the market \n";
    }

    // Display asset list
    void logAssetChanges() const {
        for (const auto& asset : assets) {
            std::cout << "Market | " << asset->getTicker() << " price updated: " << asset->getPrice() << "\n";
        }
    }

    // Simulate price changes
    void simulatePriceChanges(std::mt19937 gen) {
        const double PRICE_CHANGE_FACTOR = 0.01;
        const int RAND_RANGE = 11;
        const int RAND_OFFSET = 5;

        std::cout << "Generating stock prices..." << "\n";
        std::uniform_int_distribution<> dis(0, RAND_RANGE - 1);
        for (const auto& asset : assets) {
            double price = asset->getPrice();
            double change = PRICE_CHANGE_FACTOR * (dis(gen) - RAND_OFFSET);
            asset->setPrice(price + (price * change));
        }
        logAssetChanges();
    }
};

class Portfolio {
private:
    std::vector<std::unique_ptr<Asset>> assets;
public:
    // Update portfolio
    void updatePortfolio(std::unique_ptr<Asset> asset) {
        // Get ticker of asset
        std::string ticker = asset->getTicker();

        // Check if asset is already owned using binary search
        auto it = std::lower_bound(assets.begin(), assets.end(), ticker, [](const auto& asset, const std::string& ticker) {
            return asset->getTicker() < ticker;
        });
        if (it != assets.end() && (*it)->getTicker() == ticker) {
            // Asset already owned, update existing quantity and price
            (*it)->setQuantity((*it)->getQuantity() + asset->getQuantity());
            double newPrice = ((*it)->getPrice() + asset->getPrice()) / 2; // Update average price of existing asset in portfolio after additional purchase.
            (*it)->setPrice(newPrice);
        } else {
            // Asset not owned, insert new asset into portfolio
            assets.insert(it, std::make_unique<Asset>(asset->getTicker(), asset->getPrice(), asset->getQuantity()));
        }
    }

    // Display portfolio
    void displayPortfolio(const Market& market, int id) {
        const char currency_symbol = 156;
        std::cout << "Trader " << id << "'s Portfolio: " << "\n";
        double totalProfit = 0;
        // Iterate through portfolio and display each asset by finding the corresponding asset in the marketa and calculating profit
        for (const auto& asset : assets) {
            // Find corresponding asset in market
            const Asset* foundAsset = market.getAssetByTicker(asset->getTicker());
            
            // Calculate profit
            double profit = (foundAsset->getPrice() - asset->getPrice()) * asset->getQuantity();

            // Display asset and profit
            std::cout << "bought " << asset->getQuantity() << " " << asset->getTicker() << " shares for " << currency_symbol << asset->getPrice() << ", position is now worth " << currency_symbol << foundAsset->getPrice() << " for a profit of " << currency_symbol << profit << "\n";

            // Update total profit
            totalProfit += profit;
        }
        std::cout << "Total portfolio value: " << currency_symbol << totalProfit << "\n";
    }
};

class Order {
public:
    // Execute order, move asset to portfolio
    char currency_symbol = 156;
    void executeOrder(Portfolio& portfolio, std::unique_ptr<Asset> asset, int id) {
        std::cout << "Trader " << id << " | bought " << asset->getQuantity() << " " << asset->getTicker() << " @ " << currency_symbol << asset->getPrice() << "\n";
        portfolio.updatePortfolio(std::move(asset));
    }
};

class Trader {
private:
    Portfolio portfolio;
    const int id;

public:
    // Constructor
    Trader(int id) : id(id) {}

    // Add assets to portfolio
    void considerOrder(const Market& market, std::mt19937 gen) {
        // Constants
        const int MAX_QUANTITY = 10;
        const int MIN_QUANTITY = 1;

        // Random number generators
        std::uniform_int_distribution<> dis(0, 1);
        std::uniform_int_distribution<> dis_quantity(MIN_QUANTITY, MAX_QUANTITY);

        // Iterate through market assets
        for (const auto& asset : market.getAssets()) {
            // Decide whether or not to buy this asset
            if (dis(gen) == 0) {
                // Decide how much to buy
                int quantity = dis_quantity(gen);

                // Execute order
                Order order;
                order.executeOrder(portfolio, std::make_unique<Asset>(asset->getTicker(), asset->getPrice(), quantity), id);
            }
        }
    }

    // Display portfolio
    void displayPortfolio(const Market& market) {
        portfolio.displayPortfolio(market, id);
    }
};

class Simulation {
public:
    // Run simulation
    void run(Market& market, Trader& trader1, Trader& trader2, int iterations, std::mt19937 gen1, std::mt19937 gen2) {
        for (int i = 0; i < iterations; i++) {
            market.simulatePriceChanges(gen1);
            trader1.considerOrder(market, gen1);
            trader2.considerOrder(market, gen2);
        }
    }
};

int main() {
    // Seed random number generator
    std::random_device rd;
    std::mt19937 gen1(rd());
    std::mt19937 gen2(rd());

    // Create instances
    Market market;
    Trader trader1 = Trader(1);
    Trader trader2 = Trader(2);
    Simulation simulation;

    // Add assets to market, clean this up later
    market.addAsset(std::make_unique<Asset>("AAPL", 189.14, 0));
    market.addAsset(std::make_unique<Asset>("MSFT", 401.54, 0));
    market.addAsset(std::make_unique<Asset>("GOOGL", 141.42, 0));
    market.addAsset(std::make_unique<Asset>("AMZN", 145.85, 0));
    market.addAsset(std::make_unique<Asset>("TSLA", 249.10, 0));
    market.addAsset(std::make_unique<Asset>("META", 324.44, 0));
    market.addAsset(std::make_unique<Asset>("NFLX", 441.32, 0));
    market.addAsset(std::make_unique<Asset>("NVDA", 610.31, 0));
    market.addAsset(std::make_unique<Asset>("SPX", 5000.00, 0));

    // Run simulation
    char currency_symbol = Constants::currency_symbol;
    int iterations = Constants::iterations;
    simulation.run(market, trader1, trader2, iterations, gen1, gen2);

    // Display results
    trader1.displayPortfolio(market);
    trader2.displayPortfolio(market);

    return 0;
}