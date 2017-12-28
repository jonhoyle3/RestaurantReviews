#ifndef JHTSUSER_H
#define JHTSUSER_H
//
//  JHTSUser.h
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSDatabaseObject.h"
#include "JHTSRestaurant.h"

class JHTSUser : public JHTSDatabaseObject
{
    public:
        // Constructors
        JHTSUser(const char *lastName, const char *firstName, int zipCode = 0);
        JHTSUser(const string &lastName, const string &firstName, int zipCode = 0);

        // Destructor
        virtual ~JHTSUser();

        // public methods
        virtual JHTSGUID addRestaurant(const string &name, int zipCode);
        virtual JHTSGUID addReview(int starRating, const string &reviewText, JHTSGUID restaurantGUID);
        virtual JHTSGUID addReview(int starRating, const string &reviewText, const string &restName, int restZip);
        virtual void deleteReview(JHTSGUID reviewGUID);
        set<JHTSGUID> listOfReviews() const;

        inline virtual JHTSGUID addReview(int starRating, const char *reviewText, const char *restName, int restZip)
        { return addReview(starRating, string(reviewText), string(restName), restZip); }

        inline virtual JHTSGUID addReview(int starRating, string reviewText, const JHTSRestaurant &restaurant)
        { return addReview(starRating, reviewText, restaurant.guid()); }

        // Accessor methods
        inline string getLastName() const { return mLastName; }
        inline string getFirstName() const { return mFirstName; }
        inline int getZipCode() const { return mZipCode; }
        inline void setLastName(const char *lastName) { setLastName(string(lastName)); }
        inline void setFirstName(const char *firstName) { setFirstName(string(firstName)); }
        inline void setLastName(const string &lastName) { mLastName = lastName; }
        inline void setFirstName(const string &firstName) { mFirstName = firstName; }
        inline void setZipCode(int zipCode) { mZipCode = zipCode; }
    
        // Static methods
        static set<JHTSGUID> getUsersWithName(const string &lastName, const string &firstName, int zipCode = 0);
        static set<JHTSGUID> getUsersInZipCode(int zipCode, int numMilesRange = 0);
    
        inline static set<JHTSGUID> getUsersWithName(const char *lastName, const char *firstName, int zipCode = 0)
        { return getUsersWithName(string(lastName), string(firstName), zipCode); }
    
        inline static JHTSUser *getUser(JHTSGUID guid)
        { return dynamic_cast<JHTSUser *>(objectAtGUID(guid)); }

        inline static set<JHTSGUID> listOfUsers()
        { return listByType<JHTSUser>(); }

    protected:
        virtual void print(ostream &stream) const;
        virtual bool isValid() const;

    private:
        // private methods
        void initUser(const string &lastName, const string &firstName, int zipCode);

        // private data
        string mLastName;
        string mFirstName;
        int mZipCode;
        set<JHTSGUID> *mCachedListOfReviews;
};

#endif // JHTSUSER_H
