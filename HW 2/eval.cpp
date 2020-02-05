//
//  eval.cpp
//  CS32 Homework 2 Zachary Chang
//
//  Created by Zachary Chang on 1/31/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

//#include lines you need, including "Map.h"
//
//declarations of any additional functions you might have written
//        to help you evaluate an expression

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool infixToPostfix (string infix, string& post, bool& existInMap, const Map& values);
bool calcFix(string& postfix, const Map& values, int& evalTo);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    bool existMap = true;
    if (!infixToPostfix(infix, postfix, existMap, values))
    {
        return 1;
    }
    
    if (existMap == false)
    {
        return 2;
    }
    
    int answer;
    if (!calcFix(postfix, values, answer))
    {
        return 3;
    }
    result = answer;
    return 0;
}

bool infixToPostfix (string infix, string& post, bool& existInMap, const Map& values)
{
    string postFix = "";
    stack<char> opSymbols;
    
    int multDiv = 1;
    int addMin = 0;
    bool wasOperand = false;
    bool endParenth = false;
    bool startParenth = false;
    int numEnd = 0;
    int numStart = 0;
    
    bool hasChar = true;
    if (infix.length() == 0)
    {
        return false;
    }
    
    for (int i = 0; i < infix.length(); i++)
    {
        if (isalpha(infix[i]))
        {
            break;
        }
        if (i == infix.length() - 1 && !(isalpha(infix[i])))
        {
            hasChar = false;
        }
    }
    
    for (int i = 0; i < infix.length(); i++)
    {
        if (infix[i] == '(')
        {
            numStart++;
        }
        else if (infix[i] == ')')
        {
            numEnd++;
        }
    }
    
    if ((numStart != numEnd) || !hasChar)
    {
        return false;
    }
    for (int i = 0; i < infix.length(); i++)
    {
        char current = infix[i];
        
        if (isalpha(current) && isupper(current))
        {
            return false; //IF UPPERCASE, THE INFIX IS INVALID
        }
        
        if (isalpha(current) && islower(current))
        {
            if (!values.contains(current))
            {
                existInMap = false;
            }
            if (wasOperand || endParenth)
            {
                return false;
            }
            postFix += current;
            wasOperand = true;
            startParenth = false;
        }
        
        else if (current == '(')
        {
            startParenth = true;
            if (wasOperand || endParenth)
            {
                return false;
            }
            opSymbols.push(current);
        }
        
        else if (current == ')')
        {
            endParenth = true;
            while (opSymbols.top() != '(')
            {
                postFix += opSymbols.top();
                opSymbols.pop();
            }
            opSymbols.pop();
        }
        
        else if (current == ' ')
        {
            continue;
        }
        
        else
        {
            if (i == infix.length() - 1)
            {
                return false;
            }
            if (i == 0 || !wasOperand)
            {
                return false;
            }
            if (startParenth)
            {
                return false;
            }
            wasOperand = false;
            endParenth = false;
            int precedence = 0;
            int topPrecedence = 0;
            if (current == '*' || current == '/')
            {
                precedence = multDiv;
            }
            
            else if (current == '+' || current == '-')
            {
                precedence = addMin;
            }
            
            if (!opSymbols.empty() && (opSymbols.top() == '*' || opSymbols.top() == '/'))
            {
                topPrecedence = multDiv;
            }
            
            else if (!opSymbols.empty() && (opSymbols.top() == '+' || opSymbols.top() == '-'))
            {
                topPrecedence = addMin;
            }
            while (!opSymbols.empty() && opSymbols.top() != '(' && precedence <= topPrecedence)
            {
                postFix += opSymbols.top();
                opSymbols.pop();
                
                if (!opSymbols.empty() && (opSymbols.top() == '*' || opSymbols.top() == '/'))
                {
                    topPrecedence = multDiv;
                }
                
                else if (!opSymbols.empty() && (opSymbols.top() == '+' || opSymbols.top() == '-'))
                {
                    topPrecedence = addMin;
                }
            }
            opSymbols.push(current);
        }
    }
    while (!opSymbols.empty())
    {
        postFix += opSymbols.top();
        opSymbols.pop();
    }
    post = postFix;
    return true;
}

bool calcFix(string& postfix, const Map& values, int& evalTo)
{
    stack<int> operands;
    for (int i = 0; i < postfix.length(); i++)
    {
        char curr = postfix[i];
        if (isalpha(curr))
        {
            int val = 0;
            values.get(curr, val);
            operands.push(val);
        }
        
        else
        {
            int operand2 = operands.top();
            operands.pop();
            int operand1 = operands.top();
            operands.pop();
            if (curr == '/' && operand2 == 0)
            {
                return false;
            }
            int result;
            if (curr == '/')
            {
                result = operand1/operand2;
            }
            else if (curr == '+')
            {
                result = operand1 + operand2;
            }
            
            else if (curr == '-')
            {
                result = operand1 - operand2;
            }
            
            else if (curr == '*')
            {
                result = operand1 * operand2;
            }
            operands.push(result);
        }
    }
    evalTo = operands.top();
    return true;
}
