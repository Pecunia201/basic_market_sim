# Stock Market Simulation

This project showcases a simple simulation of stock market trading. It demonstrates the use of Object-Oriented Programming (OOP) including concepts such as encapsulation and smart pointers in C++. The program simulates traders participating in the market, investing in assets, and tracking their portfolio performance over multiple iterations of price changes.

## Code Structure

- **Asset Class**: Represents market assets with ticker, price, and quantity attributes.
- **Market Class**: Manages a collection of assets and simulates price changes.
- **Portfolio Class**: Represents a trader's portfolio, capable of updating asset holdings.
- **Order Class**: Facilitates the creation and execution of orders to buy assets.
- **Trader Class**: Simulates a trader's logic to decide on buying assets and managing their portfolio.
- **Simulation Class**: Orchestrates the entire simulation, including running market price changes and trader decisions over specified iterations.

## Getting Started

To run this simulation:

1. **Clone the Repository**
   
   Clone this repository to your local machine to get started.
   ```
   git clone https://github.com/Pecunia201/basic_market_sim.git
   ```
3. **Compilation**
   
   You can compile with the following command using ``g++``:
   ```
   g++ -o main main.cpp
   ```
4. **Running the Simulation**
   
   After compilation, run the program:
   ```
   .\main
   ```

## Planned Features

- Implement derived classes for different types of assets (e.g., stocks, bonds) to showcase polymorphism.
- Add more complex decision-making algorithms for traders, like sell strategies.

## Contributing

Feel free to fork the repository and submit pull requests.
