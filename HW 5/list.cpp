void listAll(string path, const Class* c)  // two-parameter overload
{
    if(c -> subclasses().size() == 0)
    {
        cout << path + c -> name() << endl;
        return;
    }
    
    cout << path + c -> name() << endl;
    vector<Class*>::const_iterator it = c -> subclasses().begin();
    while (it != c -> subclasses().end())
    {
        string current = path + c -> name() + "=>";
        listAll(current, *it);
        it++;
    }
}
