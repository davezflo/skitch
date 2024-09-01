#ifndef __CONTEXT__
#define __CONTEXT__

#include <map>
#include <vector>
#include <string>
using namespace std;

class SKVariable;
class SKModel;
class SKEntity;
class SKContext
{
    private:
        map<string, SKVariable *> _variables;
        map<string, string> _scratchStrings;
        map<string, float> _scratchNumbers;
        SKContext *_parentContext;
        SKEntity *_currentEntity;
        SKEntity *_camera;
        SKEntity *_lighting;
        SKModel *_model;
        string _name;

    public:
        SKContext(string name);
        SKContext(string name, SKModel *model);
        SKContext(SKContext *init, SKContext *global);
        virtual ~SKContext();
        void SetParentContext(SKContext *context) {_parentContext = context;}
        SKVariable *AddVariable(SKVariable *variable);
        SKVariable *GetVariable(string name);
        vector<string> GetVariableNames();
        string GetName() {return _name;}

    public:
        void SetCurrentEntity(SKEntity *entity) {_currentEntity=entity;}

    public:
        void SetScratchString(string name, string value);
        string GetScratchString(string name);
        void SetScratchNumber(string name, float value);
        float GetScratchNumber(string name);

        bool HasScratchNumber(string name);
        bool HasScratchString(string name);

        void Dump();

};

#endif