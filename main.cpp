#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>

struct sOperator
{
    /*
    These are the operators that are supported by the calculator
    '*', '/', '+', '-'
    */
    int precendence {};
    int arguments {};  // 1 for unary, 2 for binary operations
};

std::unordered_map<char, sOperator> mapOperator {
    {'/', {4, 2}},
    {'*', {3, 2}},
    {'+', {2, 2}},
    {'-', {1, 2}},
    // {'^', {3, 2}},
    // {'!', {4, 1}}
};

struct Token
{
    std::string value {};
    enum class Type
    {
        Unknown,
        Number,
        Operator,
        Parenthesis_Open,
        Parenthesis_Close,
        // End
    } type = Type::Unknown;
    sOperator op;
};

int main()
{
    std::string expression {};
    std::cout << "enter mathematical expression : ";
    std::getline(std::cin, expression);

    std::deque<Token> holdingStack;
    std::deque<Token> outputStack;
    
    for(const char c: expression)
    {
        if(std::isdigit(c))
        {
            // Push literal numerics right into the output stack
            outputStack.push_back({std::string(1, c), Token::Type::Number});
        }
        else if(c == '(')
        {
            // pus- h to holdingStack
            holdingStack.push_front({std::string(1, c), Token::Type::Parenthesis_Open});
        }
        else if(c == ')')
        {
            while(!holdingStack.empty() && holdingStack.front().type != Token::Type::Parenthesis_Open)
            {
                outputStack.push_back(holdingStack.front());
                holdingStack.pop_front();
            }

            if(holdingStack.empty())
            {
                std::cerr << "Error! Unexpected parenthesis..." << std::endl;
                return 0;
            }
            // remove corresponding open parenthesis from holdingStack
            if(!holdingStack.empty() && holdingStack.front().type == Token::Type::Parenthesis_Open)
            {
                holdingStack.pop_front();
            }
        }
        else if(mapOperator.contains(c))
        {
            const auto& newOp = mapOperator[c];

            while(!holdingStack.empty() && holdingStack.front().type != Token::Type::Parenthesis_Open)
            {
                // ensure stack front is an operator
                
                if(holdingStack.front().type == Token::Type::Operator)
                {
                    const auto& holdingStackOp = holdingStack.front().op;

                    // precendence check
                    if(holdingStackOp.precendence >= newOp.precendence)
                    {
                        outputStack.push_back(holdingStack.front());
                        holdingStack.pop_front();
                    }
                    else
                        break;
                }
            }

            // push the new operator to the holdingStack
            holdingStack.push_front({std::string(1, c), Token::Type::Operator, newOp});
            
        }
        else
        {
            std::cerr << "Bad symbol: " << std::string(1, c) << std::endl;
            return 0;
        }
    }

    while(!holdingStack.empty())
    {
       outputStack.push_back(holdingStack.front());
       holdingStack.pop_front();
    }

    std::cout << "R.P.N = ";
    for(const auto& item: outputStack)
    {
        std::cout << item.value;
    }
    std::cout << "\n";

    return 0;
}
