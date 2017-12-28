#ifndef JHTSREVIEW_H
#define JHTSREVIEW_H
//
//  JHTSReview.h
//  Restaurant
//
//  Created by Jonathan Hoyle on 12/21/17.
//  Copyright © 2017 Jonathan Hoyle. All rights reserved.
//

#include "JHTSDatabaseObject.h"

class JHTSUser;
class JHTSRestaurant;

class JHTSReview : public JHTSDatabaseObject
{
    public:
        // Destructor
        virtual ~JHTSReview();

        // Reconstructor operators
        void operator()(int starRating, string review);

        // accessor methods
        inline int getStarRating() const { return mStarRating; }
        inline string getReviewText() const { return mReviewText; }
        inline JHTSGUID getUserGUID() const { return mUserGUID; }
        inline JHTSGUID getRestaurantGUID() const { return mRestaurantGUID; }
        inline time_t getMostRecentUpdateTime() const { return mMostRecentUpdate; }
        inline void setReviewText(const char *reviewText) { setReviewText(string(reviewText)); }
        void setStarRating(int starRating);
        void setReviewText(const string &reviewText);

        // public methods
        static JHTSReview *getReview(JHTSGUID userGUID, JHTSGUID restaurantGUID);

        inline static set<JHTSGUID> listOfReviews()
        { return listByType<JHTSReview>(); }

        inline static JHTSReview *getReview(JHTSGUID guid)
        { return dynamic_cast<JHTSReview *>(objectAtGUID(guid)); }

    protected:
        virtual void print(ostream &stream) const;
        virtual bool isValid() const;

    private:
        // Private Constructor
        JHTSReview(int starRating, string review, JHTSGUID userGUID, JHTSGUID restaurantGUID);

        // private data
        int mStarRating;
        string mReviewText;
        JHTSGUID mUserGUID;
        JHTSGUID mRestaurantGUID;
        time_t mMostRecentUpdate;
    
        // Friend class, who can create a JHTSReview object
        friend class JHTSUser;
};

#endif // JHTSREVIEW_H
