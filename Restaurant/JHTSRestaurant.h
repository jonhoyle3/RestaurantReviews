#ifndef JHTSRESTAURANT_H
#define JHTSRESTAURANT_H
//
//  JHTSRestaurant.h
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSDatabaseObject.h"

class JHTSRestaurant : public JHTSDatabaseObject
{
    public:
        // Destructor
        virtual ~JHTSRestaurant();
    
        // Public methods
        set<JHTSGUID> listOfReviews() const;
        set<JHTSGUID> listOfReviewsWithRatingOrHigher(int starRating) const;
        int averageStarRating() const;

        // Accessor methods
        inline string getName() const { return mName; }
        inline int getZipCode() const { return mZipCode; }
        inline JHTSGUID getByUserGUID() const { return mByUser; }
        inline void setName(const char *name) { setName(string(name)); }
        inline void setName(const string &name) { mName = name; }
        inline void setZipCode(int zipCode) { mZipCode = zipCode; }

        // Static methods
        static set<JHTSGUID> listOfRestaurantsByName(const string &name, int zipCode = 0);
        static set<JHTSGUID> listOfRestaurantsByZipCode(int zipCode, int mileRange = 0, int minStarRating = -1);
        static JHTSRestaurant *getRestaurant(const string &name, int zipCode);

        inline static set<JHTSGUID> listOfRestaurants()
        { return listByType<JHTSRestaurant>(); }
    
        inline static set<JHTSGUID> listOfRestaurantsByName(const char *name, int zipCode = 0)
        { return listOfRestaurantsByName(string(name), zipCode); }
        
        inline static JHTSRestaurant *getRestaurant(JHTSGUID guid)
        { return dynamic_cast<JHTSRestaurant *>(objectAtGUID(guid)); }

    protected:
        virtual void print(ostream &stream) const;
        virtual bool isValid() const;

    private:
        // Private Constructor
        JHTSRestaurant(const string &name, int zipCode, JHTSGUID byUser);

        // private data
        string mName;
        int mZipCode;
        JHTSGUID mByUser;
        set<JHTSGUID> *mCachedListOfReviews;
    
        // Friend class, who can create a JHTSRestaurant object
        friend class JHTSUser;
};

#endif // JHTSRESTAURANT_H
