using System;
using System.Collections.Generic;
using RestaurantReviews.Object;
using RestaurantReviews.Restaurant;
using RestaurantReviews.Review;

namespace RestaurantReviews.User
{
    public class JHTSUser : JHTSObject
    {
        // Properties
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public int ZipCode { get; set; }

        // Constructor
        public JHTSUser(string lastName, string firstName, int zipCode = 0)
        {
            LastName = lastName;
            FirstName = firstName;
            ZipCode = zipCode;

            if (mCachedListOfUsers != null)
                mCachedListOfUsers.Add(GUID);
        }

        // Destructor
        ~JHTSUser()
        {
            if (mCachedListOfUsers != null)
                mCachedListOfUsers.Remove(GUID);
        }

        // List of all user objects
        //  (lazy loaded, don't calculate until needed)
        static private SortedSet<int> mCachedListOfUsers = null;
        static public SortedSet<int> ListOfUsers
        {
            get
            {
                if (mCachedListOfUsers == null)
                    mCachedListOfUsers = listByType<JHTSUser>();

                return mCachedListOfUsers;
            }
        }

        // Get user by guid
        static public JHTSUser getUser(int guid)
        { return (objectAtGUID(guid) as JHTSUser); }

        // Get the user's list of reviews (cached for performance)
        private SortedSet<int> mCachedListOfReviews = new SortedSet<int>();
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

                return mCachedListOfReviews;
            }
        }

        // Add a review to a restaurant
        public int addRestaurant(string name, int zipCode)
        {
            // See if the restaurant is already in the database
            SortedSet<int> list = JHTSRestaurant.listOfRestaurantsByName(name, zipCode);

            if ((list != null) && (list.Count > 0))
                return list.Min;
            
            JHTSRestaurant restaurant = new JHTSRestaurant(name, zipCode, GUID);

            return restaurant.GUID;
        }

        // Add a review to a restaurant
        public int addReview(int starRating, string reviewText, string restName, int restZip)
        {
            int restGUID = addRestaurant(restName, restZip);

            return addReview(starRating, reviewText, restGUID);
        }

        public JHTSReview getReviewForRestaurant(int restaurantGUID)
        {
            foreach (var guid in ListOfReviews)
            {
                JHTSReview review = JHTSReview.getReview(guid);

                if ((review != null) && (review.RestaurantGUID == restaurantGUID))
                    return review;
            }

            return null;
        }

        // Add a review to a restaurant
        public int addReview(int starRating, string reviewText, int restaurantGUID)
        {
            JHTSReview review = getReviewForRestaurant(restaurantGUID);

            // If this user already has a review for this restaurant, just update it
            if (review != null)
            {
                review.StarRating = starRating;
                review.ReviewText = reviewText;
            }
            // Otherwise, create a new one
            else
            {
                review = new JHTSReview(starRating, reviewText, GUID, restaurantGUID);

                // Update the user's review cache
                if (mCachedListOfReviews != null)
                    mCachedListOfReviews.Add(review.GUID);

                // Update the JHTSRestaurant object's review cache
                JHTSRestaurant restaurant = JHTSRestaurant.getRestaurant(restaurantGUID);

                if ((restaurant != null) && (restaurant.mCachedListOfReviews != null))
                    restaurant.mCachedListOfReviews.Add(review.GUID);
            }

            return review.GUID;
        }

        // Delete a review
        public void deleteReview(int reviewGUID)
        {
            JHTSReview review = JHTSReview.getReview(reviewGUID);

            if (review != null)
            {
                // Update the user's review cache
                if (mCachedListOfReviews != null)
                    mCachedListOfReviews.Remove(reviewGUID);

                // Update the JHTSRestaurant object's review cache
                JHTSRestaurant restaurant = JHTSRestaurant.getRestaurant(review.RestaurantGUID);

                if ((restaurant != null) && (restaurant.mCachedListOfReviews != null))
                    restaurant.mCachedListOfReviews.Remove(review.GUID);

                ObjectList[reviewGUID] = null;
            }
        }

        // Get the list of users with this name
        public SortedSet<int> getUsersWithName(string lastName, string firstName, int zipCode = 0)
        {
            SortedSet<int> userListName = new SortedSet<int>();
    
            foreach (var userIndex in ListOfUsers)
            {
                JHTSUser user = JHTSUser.getUser(userIndex);

                if ((user != null) && (user.LastName == lastName) && (user.FirstName == firstName))
                    if ((zipCode == 0) || (zipCode == user.ZipCode))
                        userListName.Add(user.GUID);
            }

            return userListName;
        }

        // Get the list of users in this zip code
        public SortedSet<int> getUsersInZipCode(int zipCode, int numMilesRange = 0)
        {
            SortedSet<int> userListZipCode = new SortedSet<int>();

            foreach (var userGUID in ListOfUsers)
            {
                JHTSUser user = JHTSUser.getUser(userGUID);

                if (ZipCodeDistance(user.ZipCode, zipCode) <= numMilesRange)
                    userListZipCode.Add(user.GUID);
            }

            return userListZipCode;
        }

        // Clear the object's cached data
        protected override void clearCache()
        {
            base.clearCache();

            if (mCachedListOfUsers != null)
            {
                mCachedListOfUsers.Clear();
                mCachedListOfUsers = null;
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
                if (review.UserGUID == GUID)
                    mCachedListOfReviews.Add(review.GUID);
        }

        // Validity check
        protected override bool IsValid
        {
            get
            {
                if (!base.IsValid)
                    return false;

                if (mCachedListOfReviews != null)
                {
                    foreach (var reviewGUID in mCachedListOfReviews)
                    {
                        JHTSReview review = JHTSReview.getReview(reviewGUID);

                        if ((review == null) || (review.UserGUID != GUID))
                            return false;

                        JHTSRestaurant restaurant = JHTSRestaurant.getRestaurant(review.RestaurantGUID);

                        if (restaurant == null)
                            return false;
                    }
                }

                return true;
            }
        }

        // Writing out contents
        public override string ToString()
        {
            string output = base.ToString() + Environment.NewLine;

            SortedSet<int> reviewList = ListOfReviews;

            output += "Name: " + FirstName + " " + LastName + Environment.NewLine;
            output += "Zip: " + ZipCode + Environment.NewLine;
            output += "Reviews: ";

            foreach (var reviewIndex in ListOfReviews)
                output += reviewIndex + ", ";
            output += Environment.NewLine;

            return output;
        }
    }
}