using System;
using System.Collections.Generic;
using RestaurantReviews.Object;
using RestaurantReviews.Review;
using RestaurantReviews.User;

namespace RestaurantReviews.Restaurant
{
    public class JHTSRestaurant : JHTSObject
    {
        // Properties
        public string Name { get; set; }
        public int ZipCode { get; set; }
        public int ByUser { get; private set; }
        protected internal SortedSet<int> mCachedListOfReviews = new SortedSet<int>();
 
        // Internal Constructor
        protected internal JHTSRestaurant(string name, int zipCode, int byUser)
        {
            Name = name;
            ZipCode = zipCode;
            ByUser = byUser;

            if (mCachedListOfRestaurants != null)
                mCachedListOfRestaurants.Add(GUID);
        }

        // Destructor
        ~JHTSRestaurant()
        {
            if (mCachedListOfRestaurants != null)
                mCachedListOfRestaurants.Remove(GUID);
        }

        // Get the restaurant object by GUID
        static public JHTSRestaurant getRestaurant(int guid)
        { return (objectAtGUID(guid) as JHTSRestaurant); }

        // List of all restaurant objects
        //  (lazy loaded, don't calculate until needed)
        static private SortedSet<int> mCachedListOfRestaurants = null;
        static public SortedSet<int> ListOfRestaurants
        {
            get
            {
                if (mCachedListOfRestaurants == null)
                    mCachedListOfRestaurants = listByType<JHTSRestaurant>();

                return mCachedListOfRestaurants;
            }
        }

        // List of reviews for a given restaurant
        //  (for performance sake, we keep a cached list of reviews)
        public SortedSet<int> ListOfReviews
        {
            get
            {
                // If we have an invalid object, recalculate the list
                if (!IsValid)
                    clearCache();

                // If we have to recalculate the cache
                if (mCachedListOfReviews == null)
                    updateCache();

                // Return our cached list
                return mCachedListOfReviews;
            }
        }

        // The average star rating, across all reviews
        //  (returning -1 if not yet reviewed).
        public int AverageStarRating
        {
            get
            {
                double sumRating = 0.0;
                int ratingCount = 0;
                JHTSReview review;

                foreach (var reviewGUID in ListOfReviews)
                {
                    review = JHTSReview.getReview(reviewGUID);
                    if (review != null)
                    {
                        sumRating += review.StarRating;
                        ratingCount++;
                    }
                }

                if (ratingCount < 1)
                    return -1;

                return (int)(sumRating / ratingCount + 0.5);
            } 
        }

        // Get the a list of restaurant list by name
        public static SortedSet<int> listOfRestaurantsByName(string name, int zipCode = 0)
        {
            SortedSet<int> restaurantNameList = new SortedSet<int>();

            foreach (var restaurantIndex in ListOfRestaurants)
            {
                JHTSRestaurant restaurant = getRestaurant(restaurantIndex);

                if ((restaurant != null) && (restaurant.Name == name))
                    if ((zipCode == 0) || (restaurant.ZipCode == zipCode))
                            restaurantNameList.Add(restaurant.GUID);
            }

            return restaurantNameList;
        }

        // Get the a list of restaurant list by zip code (and optionally minimum star rating)
        public static SortedSet<int> listOfRestaurantsByZipCode(int zipCode, int mileRange = 0, int minStarRating = -1)
        {
            SortedSet<int> restaurantZipList = new SortedSet<int>();

            foreach (var restaurantIndex in ListOfRestaurants)
            {
                JHTSRestaurant restaurant = getRestaurant(restaurantIndex);

                if (restaurant != null)
                    if ((mileRange == 0) || (ZipCodeDistance(restaurant.ZipCode, zipCode) <= mileRange))
                        if ((minStarRating == -1) || (restaurant.AverageStarRating >= minStarRating))
                            restaurantZipList.Add(restaurant.GUID);
            }

            return restaurantZipList;
        }

        // Clear the object's cached data
        protected override void clearCache()
        {
            base.clearCache();

            if (mCachedListOfRestaurants != null)
            {
                mCachedListOfRestaurants.Clear();
                mCachedListOfRestaurants = null;
            }

            if (mCachedListOfReviews != null)
            {
                mCachedListOfReviews.Clear();
                mCachedListOfReviews = null;
            }
        }

        // Update the object's cached data
        protected override void updateCache()
        {
            base.updateCache();

            mCachedListOfReviews = new SortedSet<int>();

            foreach (JHTSReview review in ObjectList)
                if (review.RestaurantGUID == this.GUID)
                    mCachedListOfReviews.Add(review.GUID);
        }

        // Validity check
        protected override bool IsValid
        {
            get
            {
                if (!base.IsValid)
                    return false;

                if (mCachedListOfReviews == null)
                    return false;

                JHTSReview review;

                foreach (var reviewGUID in mCachedListOfReviews)
                {
                    review = JHTSReview.getReview(reviewGUID);

                    if ((review == null) || (review.RestaurantGUID != GUID))
                        return false;

                    JHTSUser user = JHTSUser.getUser(review.UserGUID);

                    if (user == null)
                        return false;
                }

                return true;
            }
        }

        // Writing out contents
        public override string ToString()
        {
            string output = base.ToString() + Environment.NewLine;

            output += "Name: \"" + Name + "\"" + Environment.NewLine;
            output += "Zip: " + ZipCode + Environment.NewLine;
            output += "By: " + ByUser + Environment.NewLine;
            output += "Reviews: ";

            foreach (var reviewIndex in ListOfReviews)
                output += reviewIndex + ", ";
            output += Environment.NewLine;

            return output;
        }
    }
}