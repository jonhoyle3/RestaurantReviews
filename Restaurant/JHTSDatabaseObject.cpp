//
//  JHTSDatabaseObject.cpp
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSDatabaseObject.h"

vector<JHTSDatabaseObject *> JHTSDatabaseObject::mDatabaseObjectList;

// Normally, we would import a location library that can give real
//  distances between two zip codes, but for the purposes of this
//  exercise, we will cheat, and assume that the zipCode increments
//  or decrements with each mile.
int ZipCodeDistance(int zipCode1, int zipCode2)
{ return abs(zipCode1 - zipCode2); }


// Default Constructor
JHTSDatabaseObject::JHTSDatabaseObject()
{
    mGUID = mDatabaseObjectList.size();
    mDatabaseObjectList.push_back(this);
    mCreationTime = time(NULL);
}

// Virtual Destructor
JHTSDatabaseObject::~JHTSDatabaseObject()
{
    if (mGUID < mDatabaseObjectList.size())
        mDatabaseObjectList[mGUID] = NULL;
}

// object by GUID
JHTSDatabaseObject *JHTSDatabaseObject::objectAtGUID(JHTSGUID guid)
{
    if (guid > mDatabaseObjectList.size())
        return NULL;
    
    return mDatabaseObjectList[guid];
}

// Equality
bool operator==(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2)
{
    return (object1.mGUID == object2.mGUID);
}

// Comparison
bool operator<(const JHTSDatabaseObject &object1, const JHTSDatabaseObject &object2)
{
    return (object1.mGUID < object2.mGUID);
}

// print object
void JHTSDatabaseObject::print(ostream &stream) const
{
}

// Check Object Validity
bool JHTSDatabaseObject::isValid() const
{
    return (mGUID < mDatabaseObjectList.size());
}

// ostream
ostream &operator<<(ostream &stream, const JHTSDatabaseObject &object)
{
    stream << "Object #" << object.mGUID << " of type: " << typeid(object).name() << endl;
    object.print(stream);
    
    if (!object.isValid())
    {
        stream << "***************************" << endl;
        stream << "    Fails Validity Test!" << endl;
        stream << "***************************" << endl;
    }
    
    return stream;
}

// ostream
void printObjects()
{
    const int kMaxNum = (int) JHTSDatabaseObject::mDatabaseObjectList.size();
    JHTSDatabaseObject *object;
    
    for (int i = 0; i < kMaxNum; i++)
    {
        object = JHTSDatabaseObject::mDatabaseObjectList[i];
        
        cout << "*************************" << endl;
        cout << "INDEX #" << i << endl;
        if (object == NULL)
            cout << "NULL" << endl;
        else
            cout << *object;
        cout << "*************************" << endl << endl;
    }
}
