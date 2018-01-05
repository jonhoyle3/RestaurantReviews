using System;
using RestaurantReviews.Object;
using RestaurantReviews.User;

namespace RestaurantReviews
{
    class MainClass
    {
        public static void JHTSTest()
        {
            JHTSObject.printObjects();

            JHTSUser jon = new JHTSUser("Hoyle", "Jon", 15228);
            JHTSObject.printObjects();

            JHTSUser steve = new JHTSUser("Jobs", "Steve", 15228);
            JHTSObject.printObjects();

            int mcD = jon.addRestaurant("MacDonalds", 15201);
            JHTSObject.printObjects();

            int wen = jon.addRestaurant("Wendys", 15201);
            JHTSObject.printObjects();

            int mcDReview = jon.addReview(3, "It was okay.", mcD);
            JHTSObject.printObjects();

            int mcDReview2 = jon.addReview(4, "It was better.", mcD);
            JHTSObject.printObjects();

            int bkReview = steve.addReview(2, "Eh.", "Burger King", 15102);
            JHTSObject.printObjects();

            int mcDReview3 = steve.addReview(1, "No", mcD);
            JHTSObject.printObjects();

            steve.deleteReview(mcDReview3);
            JHTSObject.printObjects();
        }

        public static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            {
                JHTSTest();
            }
            Console.WriteLine("Goodbye World!");
        }
    }
}