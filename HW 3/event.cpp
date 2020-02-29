//
//  main.cpp
//  CS32 Homework 3 Zachary Chang
//
//  Created by Zachary Chang on 2/5/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

//#include <iostream>
//#include <string>
//using namespace std;

//Your declarations and implementations would go here
class Event
{
public:
    Event(string title)
    {
        m_title = title;
    }

    virtual string need() const = 0;

    string name() const
    {
        return m_title;
    }

    virtual bool isSport() const
    {
        return true;
    }

    virtual ~Event()
    {}

private:
    string m_title;
};

class BasketballGame : public Event
{
public:
    BasketballGame(string gameTeams)
    :Event(gameTeams)
    {}

    virtual string need() const
    {
        return "hoops";

    }
    virtual ~BasketballGame()
    {
        cout << "Destroying the " << name() << " basketball game" << endl;
    }
};

class Concert : public Event
{
public:
    Concert(string groupName, string type)
    :Event(groupName)
    {
        m_type = type;
    }

    virtual bool isSport() const
    {
        return false;
    }

    virtual string need() const
    {
        return "a stage";
    }

    virtual ~Concert()
    {
        cout << "Destroying the " << name() << " " << m_type << " concert" << endl;
    }
private:
    string m_type;
};

class HockeyGame : public Event
{
public:
    HockeyGame(string gameTeams)
    :Event(gameTeams)
    {}

    virtual string need() const
    {
        return "ice";
    }

    virtual ~HockeyGame()
    {
        cout << "Destroying the " << name() << " hockey game" << endl;
    }
};

/*
void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
    cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}

int main()
{
    Event* events[4];
    events[0] = new BasketballGame("Lakers vs. Suns");
      // Concerts have a name and a genre.
    events[1] = new Concert("Banda MS", "banda");
    events[2] = new Concert("KISS", "hard rock");
    events[3] = new HockeyGame("Kings vs. Flames");

    cout << "Here are the events." << endl;
    for (int k = 0; k < 4; k++)
    display(events[k]);

      // Clean up the events before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
    delete events[k];
}
*/
