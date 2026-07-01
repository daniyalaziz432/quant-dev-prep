//trading order system
# include<iostream>
#include<string>
struct TradeRecord
{
    int         orderId;
    std::string symbol;
    double      executedPrice;
    int         executedQty;
    char        side;
  
};
class order {
public:
    //constructors
    order() :id_(0), symbol_(""), price_(0.0), quantity_(0), side_('B'), isFilled_(false) {}
    order(int id,const std::string& symbol, double price, int qty, char side) {
        id_ = id;
        symbol_ = symbol;
        price_ = price > 0.0 ? price : 0.0;
        quantity_ = qty > 0 ? qty : 0;
        side_ = (side == 'B' || side == 'S') ? side : 'B';
        isFilled_ = false;
    }
    //destructor
    ~order() {};
    // Getters
    int getId() const { return id_; }
    const std::string& getSymbol() const { return symbol_; }
    double  getPrice() const { return price_; }
    int    getQuantity()const { return quantity_; }
    char    getSide() const { return side_; }
    bool     isFilled() const { return isFilled_; }
    double   getValue() const { return price_ * quantity_; }
    //Behavior
    bool isValid()const {
        return price_ > 0.0 && quantity_ > 0 && !symbol_.empty() && (side_ == 'S' || side_=='B');
    }
    void fill() {
        if (isValid()) {
            isFilled_ = true;
        }
    }
    //output
    void print()const {
        std::cout << "id: " << id_ << " " << "symbol: " << symbol_ << " " << "price: " << price_ << " " << "quantity: " << quantity_ << " " << "side: " << side_ << " " <<( isFilled_ ? "[Filled]" : "[open]") << std::endl;

    }

private:
    int         id_;
    std::string symbol_;
    double      price_;
    int         quantity_;
    char        side_;
    bool        isFilled_;
};
class OrderBook {
public:
    //constructor
    OrderBook() :count_(0), nextId_(1001) {
        std::cout << "[order book created]\n";
    }
    //destructor
    ~OrderBook() {
        std::cout << "[order book destroyed and " << count_ << " orders cleared]\n";
    }
    //mutators
    bool addOrder(const std::string& symbol, double price, int qty, char side) {
        if (count_ == MAX_ORDERS) {
            std::cout << "[order book is full]\n";
        }
        order o(nextId_++, symbol, price, qty, side);
        if (!o.isValid()) {
            std::cout << "Not a valid order-----rejected\n";
        }
        orders_[count_++] = o;
        std::cout << "order: " << o.getId() << " added\n";
        return true;

    }
    bool fillOrder(int id)
    {
        for (int i = 0; i < count_; i++)
        {
            if (orders_[i].getId() == id)
            {
                orders_[i].fill();
                std::cout << "  Order " << id << " filled\n";
                return true;
            }
        }
        std::cout << "  Order " << id << " not found\n";
        return false;
    }
    //queries
    //number of orders
    int count()const { return count_; }
    //number of orders filled
    int countFilled() const {
        int n = 0;
        for (int i = 0;i < count_;i++) {
            if (orders_[i].isFilled()) {
                n++;
            }
        }
        return n;
    }
    //number of open orders
    int countOpen() const { return count_ - countFilled(); }
    // valve of total orders
    double totalValue() const
    {
        double sum = 0.0;
        for (int i = 0; i < count_; i++)
            sum += orders_[i].getValue();
        return sum;
    }
    //valve of filled orders
    double filledValue() const
    {
        double sum = 0.0;
        for (int i = 0; i < count_; i++)
            if (orders_[i].isFilled())
                sum += orders_[i].getValue();
        return sum;
    }
    //finding best order
    const order* bestOrder() const
    {
        if (count_ == 0) return nullptr;

        int best = 0;
        for (int i = 1; i < count_; i++)
            if (orders_[i].getValue() > orders_[best].getValue())
                best = i;

        return &orders_[best];
    }
    // output function
     //stats output function
    void printStats() const {
        std::cout << "total orders: " << count() << std::endl;
        std::cout << "open orders: " << countOpen() << std::endl;
        std::cout << "filled orders: " << countFilled()<< std::endl;
        std::cout << "  Total Value:    " << totalValue() << std::endl;
        std::cout << "  Filled Value:   " << filledValue() << std::endl;

        const order* best = bestOrder();
        if (best != nullptr)
        {
            std::cout << "  Best Order:     ID "
                << best->getId() << " _ "
                << best->getSymbol() << " _ "
                << best->getValue() << "\n";
        }
    }
    //printing all
    void printAll()const {
        std::cout << "...................ORDER BOOK............................\n";
        if (count_ == 0) {
            std::cout << "Order Book is empty" << std::endl;
        }
        else
        {
            for (int i = 0; i < count_; i++)
                orders_[i].print();
        }
        std::cout << "......................stats....................................\n";
        printStats();

    }

private:
    static const int MAX_ORDERS = 20;
    order orders_[MAX_ORDERS];
    int   count_;
    int   nextId_;

};
// read order from user
bool readOrder(std::string& symbol, double& price, int& qty, char& side) {
    std::cout << "  Symbol (or 'done' to stop): ";
    std::cin >> symbol;
    if (symbol == "done") {
        return false;
    }

    std::cout << "  Side (B/S):                 ";
    std::cin >> side;

    std::cout << "  Price:                      ";
    std::cin >> price;

    std::cout << "  Quantity:                   ";
    std::cin >> qty;

    return true;
}

//main
int main() {
    std::cout << "===== Trading Order System =====\n\n";
    OrderBook book;

    //adding orders interactively
    std::cout << "\n--- Add Orders ---\n";
    std::cout << "(Type 'done' as symbol to stop)\n\n";

    std::string symbol;
    double      price = 0.0;
    int         qty = 0;
    char        side = 'B';

    while (readOrder(symbol, price, qty, side))
    {
        book.addOrder(symbol, price, qty, side);
        std::cout << "\n";
    }

    //printing current book
    book.printAll();
    //filling some orders
    if (book.count() > 0)
    {
        std::cout << "\n--- Fill Orders ---\n";
        std::cout << "Enter order IDs to fill (-1 to stop):\n";

        int fillId = 0;
        while (true)
        {
            std::cout << "  Fill order ID: ";
            std::cin >> fillId;

            if (fillId == -1) break;

            book.fillOrder(fillId);
        }
        //final report
        std::cout << "\n--- Final Report ---\n";
        book.printAll();

        std::cout << "\n===== Session Complete =====\n";
        return 0;
    }

}
