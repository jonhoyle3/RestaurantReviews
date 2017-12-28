#ifndef JHTSDATABASEOBJECT_H
#define JHTSDATABASEOBJECT_H
//
//  JHTSDatabaseObject.h
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include <stdint.h>
#include <time.h>

#include <vector>
#include <set>
#include <string>
#include <iostream>

using namespace std;

typedef uint64_t JHTSGUID;


// Utility function for distances
int ZipCodeDistance(int zipCode1, int zipCode2);


class JHTSDatabaseObject
{
    public:
        // Constructor & Destructor
        JHTSDatabaseObject();
        virtual ~JHTSDatabaseObject();
    
        // GUID identifier
        inline JHTSGUID guid() const { return mGUID; }
        inline time_t creationTime() const { return mCreationTime; }

        // Lookup
        static JHTSDatabaseObject *objectAtGUID(JHTSGUID guid);
    
        // Comparison operator
        friend bool operator==(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2);
        friend bool operator<(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2);
        friend ostream &operator<<(ostream &stream, const JHTSDatabaseObject &object);
        friend void printObjects();

    protected:
        virtual void print(ostream &stream) const;
        virtual bool isValid() const;
        static vector<JHTSDatabaseObject *> mDatabaseObjectList;
    
        template <typename subClassType>
        static set<JHTSGUID> listByType();

        JHTSGUID mGUID;

    private:
        // Private methods
        time_t mCreationTime;
};

// Comparison operators
inline bool operator!=(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2)
{ return !(object1 == object2); }

inline bool operator>=(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2)
{ return !(object1 < object2); }

inline bool operator<=(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2)
{ return (object1 < object2) || (object1 == object2); }

inline bool operator>(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2)
{ return !(object1 <= object2); }

template <typename subClassType>
set<JHTSGUID> JHTSDatabaseObject::listByType()
{
    set<JHTSGUID> subClassList;
    subClassType *subClass;

    for (auto ptr = mDatabaseObjectList.begin(); ptr != mDatabaseObjectList.end(); ptr++)
    {
        subClass = dynamic_cast<subClassType *>(*ptr);
        
        if (subClass != NULL)
            subClassList.insert(subClass->mGUID);
    }
    
    return subClassList;
}

ostream &operator<<(ostream &stream, const JHTSDatabaseObject &object);
void printObjects();

#endif // JHTSDATABASEOBJECT_H
