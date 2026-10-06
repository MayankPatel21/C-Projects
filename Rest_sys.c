#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FOOD 100
#define MAX_CART 50
#define MAX_TABLES 20
#define MAX_INVENTORY 100
#define MAX_EMPLOYEES 50

#define MENU_FILE "menu.dat"
#define ORDER_FILE "orders.dat"
#define ADMIN_FILE "admin.dat"
#define TABLE_FILE "tables.dat"
#define INVENTORY_FILE "inventory.dat"
#define EMPLOYEE_FILE "employees.dat"
#define RESERVATION_FILE "reservations.dat"
#define FEEDBACK_FILE "feedback.dat"
#define RECEIPT_FOLDER "receipts"

/* =========================================================
                         STRUCTURES
   ========================================================= */

/* id : admin password : admin*/
struct Food
{
    int id;
    char name[50];
    char category[30];
    float price;
    int available;
};

struct CartItem
{
    int foodId;
    char name[50];
    float price;
    int quantity;
};

struct Order
{
    int orderId;
    char customerName[50];
    int tableNo;
    float subtotal;
    float gst;
    float discount;
    float finalAmount;
    char paymentMethod[20];
    char status[20];
    char date[20];
};

struct RestaurantTable
{
    int tableNo;
    int capacity;
    char status[20];
};

struct Inventory
{
    int id;
    char name[50];
    float quantity;
    char unit[20];
    float minimumStock;
};

struct Employee
{
    int id;
    char name[50];
    char role[30];
    float salary;
    char phone[20];
};

struct Reservation
{
    int reservationId;
    char customerName[50];
    char phone[20];
    int tableNo;
    char date[20];
    char time[20];
    int guests;
    char status[20];
};

struct Feedback
{
    int orderId;
    char customerName[50];
    int rating;
    char comment[200];
};


/* =========================================================
                      GLOBAL VARIABLES
   ========================================================= */

struct Food menu[MAX_FOOD];
struct CartItem cart[MAX_CART];

struct RestaurantTable tables[MAX_TABLES];
struct Inventory inventory[MAX_INVENTORY];
struct Employee employees[MAX_EMPLOYEES];

int foodCount = 0;
int cartCount = 0;
int tableCount = 0;
int inventoryCount = 0;
int employeeCount = 0;


/* =========================================================
                    FUNCTION DECLARATIONS
   ========================================================= */

/* Initialization */
void initializeSystem();

/* Password */
void loadPassword(char password[]);
void savePassword(char password[]);
void changePassword();
void forgotPassword();

/* Login */
void adminLogin();
void adminMenu();
void customerMenu();

/* Food */
void loadMenu();
void saveMenu();
void displayMenu();
void addFood();
void updateFood();
void deleteFood();
void searchFood();
void foodCategories();
void updateFoodAvailability();

/* Cart */
void addToCart();
void viewCart();
void removeFromCart();
void clearCart();

/* Billing */
void generateBill(char customerName[], int tableNo);
void saveOrder(struct Order order);
int generateOrderID();
void choosePayment(char paymentMethod[]);
void applyCoupon(float subtotal, float *discount);

/* Orders */
void viewOrders();
void searchOrder();
void updateOrderStatus();
void customerOrderHistory();

/* Tables */
void initializeTables();
void loadTables();
void saveTables();
void displayTables();
void updateTableStatus();
void tableManagement();

/* Reservations */
void makeReservation();
void viewReservations();
void cancelReservation();
void reservationManagement();

/* Inventory */
void loadInventory();
void saveInventory();
void displayInventory();
void addInventory();
void updateInventory();
void useInventory();
void lowStockAlert();
void inventoryManagement();

/* Employees */
void loadEmployees();
void saveEmployees();
void addEmployee();
void viewEmployees();
void updateEmployee();
void deleteEmployee();
void employeeManagement();

/* Reports */
void salesReport();
void dailySalesReport();

/* Feedback */
void giveFeedback();
void viewFeedback();

/* Receipt */
void saveReceipt(struct Order order);


/* =========================================================
                         MAIN
   ========================================================= */

int main()
{
    int choice;

    initializeSystem();

    while (1)
    {
        printf("\n\n");
        printf("====================================================\n");
        printf("           RESTAURANT MANAGEMENT SYSTEM\n");
        printf("====================================================\n");
        printf("1. Admin Login\n");
        printf("2. Customer\n");
        printf("3. Exit\n");
        printf("----------------------------------------------------\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                adminLogin();
                break;

            case 2:
                customerMenu();
                break;

            case 3:
                printf("\nThank you for using the system!\n");
                return 0;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}


/* =========================================================
                    SYSTEM INITIALIZATION
   ========================================================= */

void initializeSystem()
{
    loadMenu();
    loadTables();
    loadInventory();
    loadEmployees();

    /*
       Create receipts directory on systems
       where this command is available.
    */

    system("mkdir receipts 2>nul");
}


/* =========================================================
                       PASSWORD SYSTEM
   ========================================================= */

void loadPassword(char password[])
{
    FILE *fp;

    fp = fopen(ADMIN_FILE, "r");

    if (fp == NULL)
    {
        strcpy(password, "1234");
        savePassword(password);
        return;
    }

    fscanf(fp, "%s", password);

    fclose(fp);
}


void savePassword(char password[])
{
    FILE *fp;

    fp = fopen(ADMIN_FILE, "w");

    if (fp == NULL)
    {
        printf("\nError saving password!\n");
        return;
    }

    fprintf(fp, "%s", password);

    fclose(fp);
}


void changePassword()
{
    char newPassword[50];
    char confirmPassword[50];

    printf("\n");
    printf("====================================================\n");
    printf("                  CHANGE PASSWORD\n");
    printf("====================================================\n");

    printf("Enter new password (0 = Cancel): ");
    scanf("%49s", newPassword);

    if (strcmp(newPassword, "0") == 0)
    {
        printf("\nPassword change cancelled.\n");
        return;
    }

    printf("Confirm new password (0 = Cancel): ");
    scanf("%49s", confirmPassword);

    if (strcmp(confirmPassword, "0") == 0)
    {
        printf("\nPassword change cancelled.\n");
        return;
    }

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printf("\nPasswords do not match! Password was not changed.\n");
        return;
    }

    savePassword(newPassword);

    printf("\nPassword changed successfully!\n");
}


void forgotPassword()
{
    char newPassword[50];
    char confirmPassword[50];

    printf("\n");
    printf("====================================================\n");
    printf("                  FORGOT PASSWORD\n");
    printf("====================================================\n");
    printf("You can reset the admin password here.\n");

    printf("\nEnter new password (0 = Cancel): ");
    scanf("%49s", newPassword);

    if (strcmp(newPassword, "0") == 0)
    {
        printf("\nPassword reset cancelled.\n");
        return;
    }

    if (strlen(newPassword) == 0)
    {
        printf("\nPassword cannot be empty!\n");
        return;
    }

    printf("Confirm new password (0 = Cancel): ");
    scanf("%49s", confirmPassword);

    if (strcmp(confirmPassword, "0") == 0)
    {
        printf("\nPassword reset cancelled.\n");
        return;
    }

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printf("\nPasswords do not match! Password was not changed.\n");
        return;
    }

    savePassword(newPassword);

    printf("\nPassword reset successfully!\n");
    printf("Your new password has been saved and will be used for the next login.\n");
}


/* =========================================================
                         ADMIN LOGIN
   ========================================================= */

void adminLogin()
{
    char username[50];
    char password[50];
    char savedPassword[50];
    int choice;

    while (1)
    {
        printf("\n");
        printf("====================================================\n");
        printf("                    ADMIN LOGIN\n");
        printf("====================================================\n");
        printf("1. Login\n");
        printf("2. Forgot Password\n");
        printf("0. Back\n");
        printf("----------------------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 0)
        {
            return;
        }
        else if (choice == 2)
        {
            forgotPassword();
            continue;
        }
        else if (choice != 1)
        {
            printf("\nInvalid choice!\n");
            continue;
        }

        loadPassword(savedPassword);

        printf("\nUsername (0 = Back): ");
        scanf("%49s", username);

        if (strcmp(username, "0") == 0)
            continue;

        printf("Password (0 = Back): ");
        scanf("%49s", password);

        if (strcmp(password, "0") == 0)
            continue;

        if (strcmp(username, "admin") == 0 &&
            strcmp(password, savedPassword) == 0)
        {
            printf("\nLogin successful!\n");
            adminMenu();
            return;
        }
        else
        {
            printf("\nInvalid username or password!\n");
        }
    }
}


/* =========================================================
                         ADMIN MENU
   ========================================================= */

void adminMenu()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("====================================================\n");
        printf("                     ADMIN PANEL\n");
        printf("====================================================\n");

        printf("1.  Food Menu Management\n");
        printf("2.  Order Management\n");
        printf("3.  Table Management\n");
        printf("4.  Reservation Management\n");
        printf("5.  Inventory Management\n");
        printf("6.  Employee Management\n");
        printf("7.  Sales Report\n");
        printf("8.  Daily Sales Report\n");
        printf("9.  View Customer Feedback\n");
        printf("10. Change Password\n");
        printf("11. Logout\n");

        printf("----------------------------------------------------\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                while (1)
                {
                    printf("\n");
                    printf("=============== FOOD MANAGEMENT ===============\n");
                    printf("1. View Menu\n");
                    printf("2. Add Food\n");
                    printf("3. Update Food\n");
                    printf("4. Delete Food\n");
                    printf("5. Search Food\n");
                    printf("6. Food Categories\n");
                    printf("7. Update Availability\n");
                    printf("8. Back\n");

                    printf("Enter choice: ");
                    scanf("%d", &choice);

                    switch (choice)
                    {
                        case 1:
                            displayMenu();
                            break;
                        case 2:
                            addFood();
                            break;
                        case 3:
                            updateFood();
                            break;
                        case 4:
                            deleteFood();
                            break;
                        case 5:
                            searchFood();
                            break;
                        case 6:
                            foodCategories();
                            break;
                        case 7:
                            updateFoodAvailability();
                            break;
                        case 8:
                            goto food_back;
                        default:
                            printf("\nInvalid choice!\n");
                    }
                }

                food_back:
                break;

            case 2:
                while (1)
                {
                    printf("\n");
                    printf("=============== ORDER MANAGEMENT ===============\n");
                    printf("1. View All Orders\n");
                    printf("2. Search Order\n");
                    printf("3. Update Order Status\n");
                    printf("4. Back\n");

                    printf("Enter choice: ");
                    scanf("%d", &choice);

                    switch (choice)
                    {
                        case 1:
                            viewOrders();
                            break;
                        case 2:
                            searchOrder();
                            break;
                        case 3:
                            updateOrderStatus();
                            break;
                        case 4:
                            goto order_back;
                        default:
                            printf("\nInvalid choice!\n");
                    }
                }

                order_back:
                break;

            case 3:
                tableManagement();
                break;

            case 4:
                reservationManagement();
                break;

            case 5:
                inventoryManagement();
                break;

            case 6:
                employeeManagement();
                break;

            case 7:
                salesReport();
                break;

            case 8:
                dailySalesReport();
                break;

            case 9:
                viewFeedback();
                break;

            case 10:
                changePassword();
                break;

            case 11:
                printf("\nAdmin logged out.\n");
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* =========================================================
                       MENU FILE HANDLING
   ========================================================= */

void loadMenu()
{
    FILE *fp;

    fp = fopen(MENU_FILE, "rb");

    if (fp == NULL)
    {
        foodCount = 10;

        menu[0] = (struct Food){1, "Burger", "Burgers", 120, 1};
        menu[1] = (struct Food){2, "Pizza", "Pizza", 250, 1};
        menu[2] = (struct Food){3, "Pasta", "Main Course", 180, 1};
        menu[3] = (struct Food){4, "French Fries", "Starters", 100, 1};
        menu[4] = (struct Food){5, "Sandwich", "Snacks", 90, 1};
        menu[5] = (struct Food){6, "Cold Drink", "Beverages", 50, 1};
        menu[6] = (struct Food){7, "Coffee", "Beverages", 80, 1};
        menu[7] = (struct Food){8, "Ice Cream", "Desserts", 100, 1};
        menu[8] = (struct Food){9, "Paneer Tikka", "Starters", 220, 1};
        menu[9] = (struct Food){10, "Veg Biryani", "Main Course", 200, 1};

        saveMenu();
        return;
    }

    foodCount = 0;

    while (foodCount < MAX_FOOD &&
           fread(&menu[foodCount],
                 sizeof(struct Food),
                 1,
                 fp))
    {
        foodCount++;
    }

    fclose(fp);
}


void saveMenu()
{
    FILE *fp;

    fp = fopen(MENU_FILE, "wb");

    if (fp == NULL)
    {
        printf("\nError saving menu!\n");
        return;
    }

    fwrite(menu,
           sizeof(struct Food),
           foodCount,
           fp);

    fclose(fp);
}


/* =========================================================
                         DISPLAY MENU
   ========================================================= */

void displayMenu()
{
    int i;

    printf("\n");
    printf("====================================================================\n");
    printf("                         RESTAURANT MENU\n");
    printf("====================================================================\n");

    printf("%-4s %-22s %-18s %-10s %-12s\n",
           "ID",
           "Food",
           "Category",
           "Price",
           "Availability");

    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < foodCount; i++)
    {
        printf("%-4d %-22s %-18s Rs.%-7.2f %-12s\n",
               menu[i].id,
               menu[i].name,
               menu[i].category,
               menu[i].price,
               menu[i].available ? "Available" : "Unavailable");
    }

    printf("====================================================================\n");
}


/* =========================================================
                           ADD FOOD
   ========================================================= */

void addFood()
{
    if (foodCount >= MAX_FOOD)
    {
        printf("\nMaximum food limit reached!\n");
        return;
    }

    menu[foodCount].id = foodCount + 1;

    printf("\nFood Name: ");
    scanf(" %[^\n]", menu[foodCount].name);

    printf("Category: ");
    scanf(" %[^\n]", menu[foodCount].category);

    printf("Price: ");
    scanf("%f", &menu[foodCount].price);

    menu[foodCount].available = 1;

    foodCount++;

    saveMenu();

    printf("\nFood added successfully!\n");
}


/* =========================================================
                         UPDATE FOOD
   ========================================================= */

void updateFood()
{
    int id;
    int i;

    displayMenu();

    printf("\nEnter Food ID: ");
    scanf("%d", &id);

    for (i = 0; i < foodCount; i++)
    {
        if (menu[i].id == id)
        {
            printf("New Food Name: ");
            scanf(" %[^\n]", menu[i].name);

            printf("New Category: ");
            scanf(" %[^\n]", menu[i].category);

            printf("New Price: ");
            scanf("%f", &menu[i].price);

            saveMenu();

            printf("\nFood updated successfully!\n");
            return;
        }
    }

    printf("\nFood ID not found!\n");
}


/* =========================================================
                         DELETE FOOD
   ========================================================= */

void deleteFood()
{
    int id;
    int i;
    int j;

    displayMenu();

    printf("\nEnter Food ID: ");
    scanf("%d", &id);

    for (i = 0; i < foodCount; i++)
    {
        if (menu[i].id == id)
        {
            for (j = i; j < foodCount - 1; j++)
            {
                menu[j] = menu[j + 1];
                menu[j].id = j + 1;
            }

            foodCount--;

            saveMenu();

            printf("\nFood deleted successfully!\n");
            return;
        }
    }

    printf("\nFood not found!\n");
}


/* =========================================================
                         SEARCH FOOD
   ========================================================= */

void searchFood()
{
    char search[50];
    int i;
    int found = 0;

    printf("\nEnter food name/category: ");
    scanf(" %[^\n]", search);

    for (i = 0; i < foodCount; i++)
    {
        if (strstr(menu[i].name, search) != NULL ||
            strstr(menu[i].category, search) != NULL)
        {
            printf("\nID: %d\n", menu[i].id);
            printf("Food: %s\n", menu[i].name);
            printf("Category: %s\n", menu[i].category);
            printf("Price: Rs. %.2f\n", menu[i].price);
            printf("Status: %s\n",
                   menu[i].available ? "Available" : "Unavailable");

            found = 1;
        }
    }

    if (!found)
        printf("\nNo food found!\n");
}


/* =========================================================
                      FOOD CATEGORIES
   ========================================================= */

void foodCategories()
{
    char category[30];
    int i;
    int found = 0;

    printf("\nEnter category: ");
    scanf(" %[^\n]", category);

    printf("\nCategory: %s\n", category);

    for (i = 0; i < foodCount; i++)
    {
        if (strcmp(menu[i].category, category) == 0)
        {
            printf("%d. %s - Rs. %.2f\n",
                   menu[i].id,
                   menu[i].name,
                   menu[i].price);

            found = 1;
        }
    }

    if (!found)
        printf("No items in this category.\n");
}


/* =========================================================
                    FOOD AVAILABILITY
   ========================================================= */

void updateFoodAvailability()
{
    int id;
    int choice;
    int i;

    displayMenu();

    printf("\nEnter Food ID: ");
    scanf("%d", &id);

    for (i = 0; i < foodCount; i++)
    {
        if (menu[i].id == id)
        {
            printf("1. Available\n");
            printf("2. Unavailable\n");

            printf("Enter choice: ");
            scanf("%d", &choice);

            if (choice == 1)
                menu[i].available = 1;
            else if (choice == 2)
                menu[i].available = 0;
            else
            {
                printf("\nInvalid choice!\n");
                return;
            }

            saveMenu();

            printf("\nAvailability updated!\n");
            return;
        }
    }

    printf("\nFood not found!\n");
}


/* =========================================================
                         CUSTOMER MENU
   ========================================================= */

void customerMenu()
{
    char customerName[50];
    int tableNo;
    int choice;

    clearCart();

    printf("\n");
    printf("====================================================\n");
    printf("                    CUSTOMER\n");
    printf("====================================================\n");

    printf("Enter your name: ");
    scanf(" %[^\n]", customerName);

    while (1)
    {
        printf("\n");
        printf("====================================================\n");
        printf("                 CUSTOMER PANEL\n");
        printf("====================================================\n");

        printf("Welcome, %s\n\n", customerName);

        printf("1. View Menu\n");
        printf("2. Search Food\n");
        printf("3. View Food Categories\n");
        printf("4. Add Food to Cart\n");
        printf("5. View Cart\n");
        printf("6. Remove Food from Cart\n");
        printf("7. Generate Bill / Checkout\n");
        printf("8. Reserve Table\n");
        printf("9. View Tables\n");
        printf("10. Order History\n");
        printf("11. Give Feedback\n");
        printf("12. Back\n");

        printf("----------------------------------------------------\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayMenu();
                break;

            case 2:
                searchFood();
                break;

            case 3:
            {
                char category[30];

                printf("\nEnter category: ");
                scanf(" %[^\n]", category);

                printf("\n--- %s ---\n", category);

                for (int i = 0; i < foodCount; i++)
                {
                    if (strcmp(menu[i].category, category) == 0)
                    {
                        printf("%d. %s - Rs. %.2f\n",
                               menu[i].id,
                               menu[i].name,
                               menu[i].price);
                    }
                }

                break;
            }

            case 4:
                addToCart();
                break;

            case 5:
                viewCart();
                break;

            case 6:
                removeFromCart();
                break;

            case 7:
                if (cartCount == 0)
                {
                    printf("\nCart is empty!\n");
                }
                else
                {
                    displayTables();

                    printf("\nEnter Table Number: ");
                    scanf("%d", &tableNo);

                    generateBill(customerName, tableNo);
                }
                break;

            case 8:
                makeReservation();
                break;

            case 9:
                displayTables();
                break;

            case 10:
                customerOrderHistory();
                break;

            case 11:
                giveFeedback();
                break;

            case 12:
                clearCart();
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* =========================================================
                      ADD MULTIPLE ITEMS
   ========================================================= */

void addToCart()
{
    int id;
    int quantity;
    int i;
    int j;
    int found;
    int alreadyInCart;
    int continueAdding;

    while (1)
    {
        found = 0;

        displayMenu();

        printf("\nEnter Food ID");
        printf(" (0 = Finish): ");

        scanf("%d", &id);

        if (id == 0)
        {
            printf("\nFinished adding items.\n");
            return;
        }

        for (i = 0; i < foodCount; i++)
        {
            if (menu[i].id == id)
            {
                found = 1;

                if (!menu[i].available)
                {
                    printf("\nThis food is currently unavailable!\n");
                    break;
                }

                printf("\nFood: %s\n",
                       menu[i].name);

                printf("Price: Rs. %.2f\n",
                       menu[i].price);

                printf("Quantity: ");
                scanf("%d", &quantity);

                if (quantity <= 0)
                {
                    printf("\nInvalid quantity!\n");
                    break;
                }

                if (cartCount >= MAX_CART)
                {
                    printf("\nCart is full!\n");
                    return;
                }

                alreadyInCart = 0;

                for (j = 0; j < cartCount; j++)
                {
                    if (cart[j].foodId == id)
                    {
                        cart[j].quantity += quantity;
                        alreadyInCart = 1;

                        printf("\nQuantity updated!\n");
                        break;
                    }
                }

                if (!alreadyInCart)
                {
                    cart[cartCount].foodId = menu[i].id;

                    strcpy(cart[cartCount].name,
                           menu[i].name);

                    cart[cartCount].price =
                        menu[i].price;

                    cart[cartCount].quantity =
                        quantity;

                    cartCount++;

                    printf("\n%s added to cart!\n",
                           menu[i].name);
                }

                break;
            }
        }

        if (!found)
        {
            printf("\nInvalid Food ID!\n");
        }

        printf("\nAdd another item?");
        printf(" (1 = Yes, 0 = No): ");

        scanf("%d", &continueAdding);

        if (continueAdding == 0)
            return;
    }
}


/* =========================================================
                          VIEW CART
   ========================================================= */

void viewCart()
{
    int i;
    float total = 0;

    if (cartCount == 0)
    {
        printf("\nYour cart is empty.\n");
        return;
    }

    printf("\n");
    printf("====================================================\n");
    printf("                      YOUR CART\n");
    printf("====================================================\n");

    printf("%-5s %-20s %-8s %-12s\n",
           "ID",
           "Item",
           "Qty",
           "Amount");

    printf("----------------------------------------------------\n");

    for (i = 0; i < cartCount; i++)
    {
        float amount =
            cart[i].price *
            cart[i].quantity;

        printf("%-5d %-20s %-8d Rs. %.2f\n",
               cart[i].foodId,
               cart[i].name,
               cart[i].quantity,
               amount);

        total += amount;
    }

    printf("----------------------------------------------------\n");
    printf("Cart Total: Rs. %.2f\n", total);
    printf("====================================================\n");
}


/* =========================================================
                     REMOVE FROM CART
   ========================================================= */

void removeFromCart()
{
    int id;
    int i;
    int j;

    viewCart();

    if (cartCount == 0)
        return;

    printf("\nEnter Food ID to remove: ");
    scanf("%d", &id);

    for (i = 0; i < cartCount; i++)
    {
        if (cart[i].foodId == id)
        {
            for (j = i; j < cartCount - 1; j++)
            {
                cart[j] = cart[j + 1];
            }

            cartCount--;

            printf("\nItem removed successfully!\n");
            return;
        }
    }

    printf("\nItem not found!\n");
}


/* =========================================================
                         CLEAR CART
   ========================================================= */

void clearCart()
{
    cartCount = 0;
}


/* =========================================================
                        PAYMENT
   ========================================================= */

void choosePayment(char paymentMethod[])
{
    int choice;

    printf("\n");
    printf("=============== PAYMENT ===============\n");

    printf("1. Cash\n");
    printf("2. UPI\n");
    printf("3. Card\n");

    printf("Select payment method: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(paymentMethod, "Cash");
            break;

        case 2:
            strcpy(paymentMethod, "UPI");
            break;

        case 3:
            strcpy(paymentMethod, "Card");
            break;

        default:
            strcpy(paymentMethod, "Cash");
            printf("\nInvalid choice. Cash selected.\n");
    }
}


/* =========================================================
                         COUPON
   ========================================================= */

void applyCoupon(float subtotal, float *discount)
{
    char coupon[30];

    printf("\nEnter coupon code");
    printf(" (or NONE): ");

    scanf("%s", coupon);

    if (strcmp(coupon, "NONE") == 0)
    {
        *discount = 0;
        return;
    }

    if (strcmp(coupon, "SAVE10") == 0)
    {
        *discount = subtotal * 0.10;
        printf("\n10%% discount applied!\n");
    }
    else if (strcmp(coupon, "SAVE20") == 0)
    {
        *discount = subtotal * 0.20;
        printf("\n20%% discount applied!\n");
    }
    else
    {
        *discount = 0;
        printf("\nInvalid coupon code!\n");
    }
}


/* =========================================================
                       GENERATE BILL
   ========================================================= */

void generateBill(char customerName[], int tableNo)
{
    struct Order order;

    int i;

    order.orderId = generateOrderID();

    strcpy(order.customerName,
           customerName);

    order.tableNo = tableNo;

    order.subtotal = 0;

    for (i = 0; i < cartCount; i++)
    {
        order.subtotal +=
            cart[i].price *
            cart[i].quantity;
    }

    order.gst =
        order.subtotal * 0.05;

    applyCoupon(order.subtotal,
                &order.discount);

    order.finalAmount =
        order.subtotal +
        order.gst -
        order.discount;

    choosePayment(order.paymentMethod);

    strcpy(order.status, "Pending");

    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        sprintf(order.date,
                "%02d/%02d/%04d",
                t->tm_mday,
                t->tm_mon + 1,
                t->tm_year + 1900);
    }

    printf("\n");
    printf("====================================================\n");
    printf("                    FINAL BILL\n");
    printf("====================================================\n");

    printf("Order ID       : %d\n",
           order.orderId);

    printf("Customer       : %s\n",
           order.customerName);

    printf("Table          : %d\n",
           order.tableNo);

    printf("Date           : %s\n",
           order.date);

    printf("----------------------------------------------------\n");

    for (i = 0; i < cartCount; i++)
    {
        printf("%-20s x %-5d Rs. %.2f\n",
               cart[i].name,
               cart[i].quantity,
               cart[i].price *
               cart[i].quantity);
    }

    printf("----------------------------------------------------\n");

    printf("Subtotal       : Rs. %.2f\n",
           order.subtotal);

    printf("GST (5%%)       : Rs. %.2f\n",
           order.gst);

    printf("Discount       : Rs. %.2f\n",
           order.discount);

    printf("Final Amount   : Rs. %.2f\n",
           order.finalAmount);

    printf("Payment        : %s\n",
           order.paymentMethod);

    printf("Status         : %s\n",
           order.status);

    printf("====================================================\n");

    saveOrder(order);
    saveReceipt(order);

    /*
       Mark table occupied/available logic.
       After checkout, table becomes occupied.
    */

    for (i = 0; i < tableCount; i++)
    {
        if (tables[i].tableNo == tableNo)
        {
            strcpy(tables[i].status,
                   "Occupied");
        }
    }

    saveTables();

    clearCart();

    printf("\nOrder placed successfully!\n");
}


/* =========================================================
                      GENERATE ORDER ID
   ========================================================= */

int generateOrderID()
{
    static int lastID = 1000;

    lastID++;

    return lastID;
}


/* =========================================================
                        SAVE ORDER
   ========================================================= */

void saveOrder(struct Order order)
{
    FILE *fp;

    fp = fopen(ORDER_FILE, "ab");

    if (fp == NULL)
    {
        printf("\nError saving order!\n");
        return;
    }

    fwrite(&order,
           sizeof(struct Order),
           1,
           fp);

    fclose(fp);
}


/* =========================================================
                       VIEW ORDERS
   ========================================================= */

void viewOrders()
{
    FILE *fp;
    struct Order order;

    fp = fopen(ORDER_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo orders found.\n");
        return;
    }

    printf("\n");
    printf("====================================================================\n");
    printf("                           ALL ORDERS\n");
    printf("====================================================================\n");

    while (fread(&order,
                 sizeof(struct Order),
                 1,
                 fp))
    {
        printf("\nOrder ID   : %d", order.orderId);
        printf("\nCustomer   : %s", order.customerName);
        printf("\nTable      : %d", order.tableNo);
        printf("\nAmount     : Rs. %.2f", order.finalAmount);
        printf("\nPayment    : %s", order.paymentMethod);
        printf("\nStatus     : %s", order.status);
        printf("\nDate       : %s", order.date);
        printf("\n--------------------------------------------");
    }

    fclose(fp);
}


/* =========================================================
                        SEARCH ORDER
   ========================================================= */

void searchOrder()
{
    FILE *fp;
    struct Order order;
    int id;
    int found = 0;

    printf("\nEnter Order ID: ");
    scanf("%d", &id);

    fp = fopen(ORDER_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo orders found.\n");
        return;
    }

    while (fread(&order,
                 sizeof(struct Order),
                 1,
                 fp))
    {
        if (order.orderId == id)
        {
            printf("\nOrder Found!\n");
            printf("Order ID: %d\n", order.orderId);
            printf("Customer: %s\n", order.customerName);
            printf("Table: %d\n", order.tableNo);
            printf("Amount: Rs. %.2f\n", order.finalAmount);
            printf("Payment: %s\n", order.paymentMethod);
            printf("Status: %s\n", order.status);
            printf("Date: %s\n", order.date);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nOrder not found!\n");
}


/* =========================================================
                    CUSTOMER ORDER HISTORY
   ========================================================= */

void customerOrderHistory()
{
    FILE *fp;
    struct Order order;
    char name[50];
    int found = 0;

    printf("\nEnter customer name: ");
    scanf(" %[^\n]", name);

    fp = fopen(ORDER_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo orders found.\n");
        return;
    }

    printf("\n========== ORDER HISTORY ==========\n");

    while (fread(&order,
                 sizeof(struct Order),
                 1,
                 fp))
    {
        if (strcmp(order.customerName,
                   name) == 0)
        {
            printf("\nOrder ID: %d", order.orderId);
            printf("\nDate: %s", order.date);
            printf("\nAmount: Rs. %.2f", order.finalAmount);
            printf("\nPayment: %s", order.paymentMethod);
            printf("\nStatus: %s\n", order.status);

            found = 1;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nNo order history found.\n");
}


/* =========================================================
                    UPDATE ORDER STATUS
   ========================================================= */

void updateOrderStatus()
{
    FILE *fp;
    struct Order order;

    int id;
    int choice;
    int found = 0;

    fp = fopen(ORDER_FILE, "rb+");

    if (fp == NULL)
    {
        printf("\nNo orders found.\n");
        return;
    }

    printf("\nEnter Order ID: ");
    scanf("%d", &id);

    while (fread(&order,
                 sizeof(struct Order),
                 1,
                 fp))
    {
        if (order.orderId == id)
        {
            found = 1;

            printf("\nCurrent Status: %s\n",
                   order.status);

            printf("\n1. Pending\n");
            printf("2. Preparing\n");
            printf("3. Ready\n");
            printf("4. Completed\n");
            printf("5. Cancelled\n");

            printf("Choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
                case 1:
                    strcpy(order.status, "Pending");
                    break;

                case 2:
                    strcpy(order.status, "Preparing");
                    break;

                case 3:
                    strcpy(order.status, "Ready");
                    break;

                case 4:
                    strcpy(order.status, "Completed");
                    break;

                case 5:
                    strcpy(order.status, "Cancelled");
                    break;

                default:
                    printf("\nInvalid choice!\n");
                    fclose(fp);
                    return;
            }

            fseek(fp,
                  -sizeof(struct Order),
                  SEEK_CUR);

            fwrite(&order,
                   sizeof(struct Order),
                   1,
                   fp);

            printf("\nOrder status updated!\n");

            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nOrder not found!\n");
}


/* =========================================================
                       TABLE SYSTEM
   ========================================================= */

void initializeTables()
{
    int i;

    tableCount = MAX_TABLES;

    for (i = 0; i < MAX_TABLES; i++)
    {
        tables[i].tableNo = i + 1;

        if (i < 5)
            tables[i].capacity = 2;
        else if (i < 15)
            tables[i].capacity = 4;
        else
            tables[i].capacity = 6;

        strcpy(tables[i].status,
               "Available");
    }

    saveTables();
}


void loadTables()
{
    FILE *fp;

    fp = fopen(TABLE_FILE, "rb");

    if (fp == NULL)
    {
        initializeTables();
        return;
    }

    tableCount = 0;

    while (tableCount < MAX_TABLES &&
           fread(&tables[tableCount],
                 sizeof(struct RestaurantTable),
                 1,
                 fp))
    {
        tableCount++;
    }

    fclose(fp);
}


void saveTables()
{
    FILE *fp;

    fp = fopen(TABLE_FILE, "wb");

    if (fp == NULL)
        return;

    fwrite(tables,
           sizeof(struct RestaurantTable),
           tableCount,
           fp);

    fclose(fp);
}


void displayTables()
{
    int i;

    printf("\n");
    printf("====================================================\n");
    printf("                    TABLE STATUS\n");
    printf("====================================================\n");

    printf("%-10s %-12s %-15s\n",
           "Table",
           "Capacity",
           "Status");

    printf("----------------------------------------------------\n");

    for (i = 0; i < tableCount; i++)
    {
        printf("%-10d %-12d %-15s\n",
               tables[i].tableNo,
               tables[i].capacity,
               tables[i].status);
    }

    printf("====================================================\n");
}


void updateTableStatus()
{
    int tableNo;
    int choice;
    int i;

    displayTables();

    printf("\nEnter table number: ");
    scanf("%d", &tableNo);

    for (i = 0; i < tableCount; i++)
    {
        if (tables[i].tableNo == tableNo)
        {
            printf("\n1. Available\n");
            printf("2. Occupied\n");
            printf("3. Reserved\n");

            printf("Choice: ");
            scanf("%d", &choice);

            if (choice == 1)
                strcpy(tables[i].status, "Available");
            else if (choice == 2)
                strcpy(tables[i].status, "Occupied");
            else if (choice == 3)
                strcpy(tables[i].status, "Reserved");
            else
            {
                printf("\nInvalid choice!\n");
                return;
            }

            saveTables();

            printf("\nTable status updated!\n");
            return;
        }
    }

    printf("\nTable not found!\n");
}


void tableManagement()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("=============== TABLE MANAGEMENT ===============\n");

        printf("1. View Tables\n");
        printf("2. Update Table Status\n");
        printf("3. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayTables();
                break;

            case 2:
                updateTableStatus();
                break;

            case 3:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* =========================================================
                    RESERVATION SYSTEM
   ========================================================= */

void makeReservation()
{
    FILE *fp;

    struct Reservation r;

    r.reservationId =
        (int)time(NULL) % 100000;

    printf("\nCustomer Name: ");
    scanf(" %[^\n]", r.customerName);

    printf("Phone: ");
    scanf("%s", r.phone);

    displayTables();

    printf("Table Number: ");
    scanf("%d", &r.tableNo);

    printf("Date (DD/MM/YYYY): ");
    scanf("%s", r.date);

    printf("Time: ");
    scanf("%s", r.time);

    printf("Number of Guests: ");
    scanf("%d", &r.guests);

    strcpy(r.status, "Reserved");

    fp = fopen(RESERVATION_FILE, "ab");

    if (fp == NULL)
    {
        printf("\nError saving reservation!\n");
        return;
    }

    fwrite(&r,
           sizeof(struct Reservation),
           1,
           fp);

    fclose(fp);

    printf("\nReservation successful!\n");
    printf("Reservation ID: %d\n",
           r.reservationId);
}


void viewReservations()
{
    FILE *fp;

    struct Reservation r;

    fp = fopen(RESERVATION_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo reservations found.\n");
        return;
    }

    printf("\n=============== RESERVATIONS ===============\n");

    while (fread(&r,
                 sizeof(struct Reservation),
                 1,
                 fp))
    {
        printf("\nReservation ID: %d", r.reservationId);
        printf("\nCustomer: %s", r.customerName);
        printf("\nPhone: %s", r.phone);
        printf("\nTable: %d", r.tableNo);
        printf("\nDate: %s", r.date);
        printf("\nTime: %s", r.time);
        printf("\nGuests: %d", r.guests);
        printf("\nStatus: %s\n", r.status);
    }

    fclose(fp);
}


void cancelReservation()
{
    FILE *fp;
    FILE *temp;

    struct Reservation r;

    int id;
    int found = 0;

    printf("\nReservation ID: ");
    scanf("%d", &id);

    fp = fopen(RESERVATION_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo reservations found.\n");
        return;
    }

    temp = fopen("temp_res.dat", "wb");

    while (fread(&r,
                 sizeof(struct Reservation),
                 1,
                 fp))
    {
        if (r.reservationId == id)
        {
            strcpy(r.status, "Cancelled");
            found = 1;
        }

        fwrite(&r,
               sizeof(struct Reservation),
               1,
               temp);
    }

    fclose(fp);
    fclose(temp);

    remove(RESERVATION_FILE);
    rename("temp_res.dat",
           RESERVATION_FILE);

    if (found)
        printf("\nReservation cancelled!\n");
    else
        printf("\nReservation not found!\n");
}


void reservationManagement()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("============= RESERVATION MANAGEMENT =============\n");

        printf("1. Make Reservation\n");
        printf("2. View Reservations\n");
        printf("3. Cancel Reservation\n");
        printf("4. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                makeReservation();
                break;

            case 2:
                viewReservations();
                break;

            case 3:
                cancelReservation();
                break;

            case 4:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* =========================================================
                     INVENTORY SYSTEM
   ========================================================= */

void loadInventory()
{
    FILE *fp;

    fp = fopen(INVENTORY_FILE, "rb");

    if (fp == NULL)
    {
        inventoryCount = 5;

        inventory[0] =
            (struct Inventory){1, "Cheese", 10, "kg", 2};

        inventory[1] =
            (struct Inventory){2, "Bread", 20, "packets", 5};

        inventory[2] =
            (struct Inventory){3, "Tomato", 15, "kg", 3};

        inventory[3] =
            (struct Inventory){4, "Potato", 25, "kg", 5};

        inventory[4] =
            (struct Inventory){5, "Coffee Beans", 5, "kg", 1};

        saveInventory();

        return;
    }

    inventoryCount = 0;

    while (inventoryCount < MAX_INVENTORY &&
           fread(&inventory[inventoryCount],
                 sizeof(struct Inventory),
                 1,
                 fp))
    {
        inventoryCount++;
    }

    fclose(fp);
}


void saveInventory()
{
    FILE *fp;

    fp = fopen(INVENTORY_FILE, "wb");

    if (fp == NULL)
        return;

    fwrite(inventory,
           sizeof(struct Inventory),
           inventoryCount,
           fp);

    fclose(fp);
}


void displayInventory()
{
    int i;

    printf("\n");
    printf("====================================================\n");
    printf("                     INVENTORY\n");
    printf("====================================================\n");

    printf("%-5s %-20s %-10s %-10s %-10s\n",
           "ID",
           "Ingredient",
           "Stock",
           "Unit",
           "Minimum");

    printf("----------------------------------------------------\n");

    for (i = 0; i < inventoryCount; i++)
    {
        printf("%-5d %-20s %-10.2f %-10s %-10.2f\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].quantity,
               inventory[i].unit,
               inventory[i].minimumStock);
    }

    printf("====================================================\n");
}


void addInventory()
{
    if (inventoryCount >= MAX_INVENTORY)
    {
        printf("\nInventory is full!\n");
        return;
    }

    inventory[inventoryCount].id =
        inventoryCount + 1;

    printf("\nIngredient Name: ");
    scanf(" %[^\n]", inventory[inventoryCount].name);

    printf("Quantity: ");
    scanf("%f", &inventory[inventoryCount].quantity);

    printf("Unit: ");
    scanf("%s", inventory[inventoryCount].unit);

    printf("Minimum Stock: ");
    scanf("%f",
          &inventory[inventoryCount].minimumStock);

    inventoryCount++;

    saveInventory();

    printf("\nInventory item added!\n");
}


void updateInventory()
{
    int id;
    int i;

    displayInventory();

    printf("\nEnter Inventory ID: ");
    scanf("%d", &id);

    for (i = 0; i < inventoryCount; i++)
    {
        if (inventory[i].id == id)
        {
            printf("New Quantity: ");
            scanf("%f",
                  &inventory[i].quantity);

            printf("New Minimum Stock: ");
            scanf("%f",
                  &inventory[i].minimumStock);

            saveInventory();

            printf("\nInventory updated!\n");
            return;
        }
    }

    printf("\nInventory item not found!\n");
}


void useInventory()
{
    int id;
    float quantity;
    int i;

    displayInventory();

    printf("\nEnter Inventory ID: ");
    scanf("%d", &id);

    for (i = 0; i < inventoryCount; i++)
    {
        if (inventory[i].id == id)
        {
            printf("Quantity used: ");
            scanf("%f", &quantity);

            if (quantity > inventory[i].quantity)
            {
                printf("\nNot enough stock!\n");
                return;
            }

            inventory[i].quantity -= quantity;

            saveInventory();

            printf("\nStock updated!\n");
            return;
        }
    }

    printf("\nItem not found!\n");
}


void lowStockAlert()
{
    int i;
    int found = 0;

    printf("\n=============== LOW STOCK ALERT ===============\n");

    for (i = 0; i < inventoryCount; i++)
    {
        if (inventory[i].quantity <=
            inventory[i].minimumStock)
        {
            printf("\n%s",
                   inventory[i].name);

            printf(" -> %.2f %s",
                   inventory[i].quantity,
                   inventory[i].unit);

            printf(" [LOW STOCK]\n");

            found = 1;
        }
    }

    if (!found)
        printf("\nAll inventory levels are normal.\n");
}


void inventoryManagement()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("=============== INVENTORY MANAGEMENT ===============\n");

        printf("1. View Inventory\n");
        printf("2. Add Inventory Item\n");
        printf("3. Update Inventory\n");
        printf("4. Use Stock\n");
        printf("5. Low Stock Alert\n");
        printf("6. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayInventory();
                break;

            case 2:
                addInventory();
                break;

            case 3:
                updateInventory();
                break;

            case 4:
                useInventory();
                break;

            case 5:
                lowStockAlert();
                break;

            case 6:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* =========================================================
                    EMPLOYEE MANAGEMENT
   ========================================================= */

void loadEmployees()
{
    FILE *fp;

    fp = fopen(EMPLOYEE_FILE, "rb");

    if (fp == NULL)
    {
        employeeCount = 0;
        return;
    }

    employeeCount = 0;

    while (employeeCount < MAX_EMPLOYEES &&
           fread(&employees[employeeCount],
                 sizeof(struct Employee),
                 1,
                 fp))
    {
        employeeCount++;
    }

    fclose(fp);
}


void saveEmployees()
{
    FILE *fp;

    fp = fopen(EMPLOYEE_FILE, "wb");

    if (fp == NULL)
        return;

    fwrite(employees,
           sizeof(struct Employee),
           employeeCount,
           fp);

    fclose(fp);
}


void addEmployee()
{
    if (employeeCount >= MAX_EMPLOYEES)
    {
        printf("\nEmployee limit reached!\n");
        return;
    }

    employees[employeeCount].id =
        employeeCount + 1;

    printf("\nEmployee Name: ");
    scanf(" %[^\n]", employees[employeeCount].name);

    printf("Role: ");
    scanf(" %[^\n]", employees[employeeCount].role);

    printf("Salary: ");
    scanf("%f",
          &employees[employeeCount].salary);

    printf("Phone: ");
    scanf("%s",
          employees[employeeCount].phone);

    employeeCount++;

    saveEmployees();

    printf("\nEmployee added successfully!\n");
}


void viewEmployees()
{
    int i;

    printf("\n");
    printf("====================================================\n");
    printf("                    EMPLOYEES\n");
    printf("====================================================\n");

    for (i = 0; i < employeeCount; i++)
    {
        printf("\nID     : %d", employees[i].id);
        printf("\nName   : %s", employees[i].name);
        printf("\nRole   : %s", employees[i].role);
        printf("\nSalary : Rs. %.2f", employees[i].salary);
        printf("\nPhone  : %s\n", employees[i].phone);
    }

    if (employeeCount == 0)
        printf("\nNo employees found.\n");
}


void updateEmployee()
{
    int id;
    int i;

    viewEmployees();

    printf("\nEmployee ID: ");
    scanf("%d", &id);

    for (i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            printf("New Role: ");
            scanf(" %[^\n]", employees[i].role);

            printf("New Salary: ");
            scanf("%f", &employees[i].salary);

            printf("New Phone: ");
            scanf("%s", employees[i].phone);

            saveEmployees();

            printf("\nEmployee updated!\n");
            return;
        }
    }

    printf("\nEmployee not found!\n");
}


void deleteEmployee()
{
    int id;
    int i;
    int j;

    viewEmployees();

    printf("\nEmployee ID: ");
    scanf("%d", &id);

    for (i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            for (j = i;
                 j < employeeCount - 1;
                 j++)
            {
                employees[j] = employees[j + 1];
                employees[j].id = j + 1;
            }

            employeeCount--;

            saveEmployees();

            printf("\nEmployee deleted!\n");
            return;
        }
    }

    printf("\nEmployee not found!\n");
}


void employeeManagement()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("=============== EMPLOYEE MANAGEMENT ===============\n");

        printf("1. Add Employee\n");
        printf("2. View Employees\n");
        printf("3. Update Employee\n");
        printf("4. Delete Employee\n");
        printf("5. Back\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addEmployee();
                break;

            case 2:
                viewEmployees();
                break;

            case 3:
                updateEmployee();
                break;

            case 4:
                deleteEmployee();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid choice!\n");
        }
    }
}


/* =========================================================
                         SALES REPORT
   ========================================================= */

void salesReport()
{
    FILE *fp;

    struct Order order;

    float totalSales = 0;
    int totalOrders = 0;
    int completed = 0;
    int cancelled = 0;

    fp = fopen(ORDER_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo sales data available.\n");
        return;
    }

    while (fread(&order,
                 sizeof(struct Order),
                 1,
                 fp))
    {
        if (strcmp(order.status,
                   "Cancelled") == 0)
        {
            cancelled++;
        }
        else
        {
            totalSales += order.finalAmount;
            totalOrders++;
        }

        if (strcmp(order.status,
                   "Completed") == 0)
        {
            completed++;
        }
    }

    fclose(fp);

    printf("\n");
    printf("====================================================\n");
    printf("                     SALES REPORT\n");
    printf("====================================================\n");

    printf("Total Orders      : %d\n",
           totalOrders);

    printf("Completed Orders  : %d\n",
           completed);

    printf("Cancelled Orders  : %d\n",
           cancelled);

    printf("Total Sales       : Rs. %.2f\n",
           totalSales);

    printf("====================================================\n");
}


/* =========================================================
                    DAILY SALES REPORT
   ========================================================= */

void dailySalesReport()
{
    FILE *fp;

    struct Order order;

    char today[20];

    float sales = 0;
    int orders = 0;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    sprintf(today,
            "%02d/%02d/%04d",
            t->tm_mday,
            t->tm_mon + 1,
            t->tm_year + 1900);

    fp = fopen(ORDER_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo orders found.\n");
        return;
    }

    while (fread(&order,
                 sizeof(struct Order),
                 1,
                 fp))
    {
        if (strcmp(order.date, today) == 0 &&
            strcmp(order.status, "Cancelled") != 0)
        {
            sales += order.finalAmount;
            orders++;
        }
    }

    fclose(fp);

    printf("\n");
    printf("====================================================\n");
    printf("                  TODAY'S SALES\n");
    printf("====================================================\n");

    printf("Date          : %s\n", today);
    printf("Orders        : %d\n", orders);
    printf("Total Sales   : Rs. %.2f\n", sales);

    printf("====================================================\n");
}


/* =========================================================
                         FEEDBACK
   ========================================================= */

void giveFeedback()
{
    FILE *fp;

    struct Feedback feedback;

    printf("\nOrder ID: ");
    scanf("%d", &feedback.orderId);

    printf("Customer Name: ");
    scanf(" %[^\n]", feedback.customerName);

    printf("Rating (1-5): ");
    scanf("%d", &feedback.rating);

    if (feedback.rating < 1 ||
        feedback.rating > 5)
    {
        printf("\nRating must be between 1 and 5.\n");
        return;
    }

    printf("Comment: ");
    scanf(" %[^\n]", feedback.comment);

    fp = fopen(FEEDBACK_FILE, "ab");

    if (fp == NULL)
    {
        printf("\nError saving feedback!\n");
        return;
    }

    fwrite(&feedback,
           sizeof(struct Feedback),
           1,
           fp);

    fclose(fp);

    printf("\nThank you for your feedback!\n");
}


void viewFeedback()
{
    FILE *fp;

    struct Feedback feedback;

    int total = 0;
    int ratingSum = 0;

    fp = fopen(FEEDBACK_FILE, "rb");

    if (fp == NULL)
    {
        printf("\nNo feedback available.\n");
        return;
    }

    printf("\n");
    printf("====================================================\n");
    printf("                  CUSTOMER FEEDBACK\n");
    printf("====================================================\n");

    while (fread(&feedback,
                 sizeof(struct Feedback),
                 1,
                 fp))
    {
        printf("\nOrder ID : %d",
               feedback.orderId);

        printf("\nCustomer : %s",
               feedback.customerName);

        printf("\nRating   : %d/5",
               feedback.rating);

        printf("\nComment  : %s\n",
               feedback.comment);

        printf("--------------------------------------------\n");

        total++;
        ratingSum += feedback.rating;
    }

    fclose(fp);

    if (total > 0)
    {
        printf("\nAverage Rating: %.2f / 5\n",
               (float)ratingSum / total);
    }
}


/* =========================================================
                       SAVE RECEIPT
   ========================================================= */

void saveReceipt(struct Order order)
{
    FILE *fp;

    char filename[100];

    sprintf(filename,
            "receipts/receipt_%d.txt",
            order.orderId);

    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("\nReceipt could not be saved.\n");
        return;
    }

    fprintf(fp,
            "=============================================\n");

    fprintf(fp,
            "          RESTAURANT RECEIPT\n");

    fprintf(fp,
            "=============================================\n");

    fprintf(fp,
            "Order ID       : %d\n",
            order.orderId);

    fprintf(fp,
            "Customer       : %s\n",
            order.customerName);

    fprintf(fp,
            "Table          : %d\n",
            order.tableNo);

    fprintf(fp,
            "Date           : %s\n",
            order.date);

    fprintf(fp,
            "---------------------------------------------\n");

    fprintf(fp,
            "Subtotal       : Rs. %.2f\n",
            order.subtotal);

    fprintf(fp,
            "GST            : Rs. %.2f\n",
            order.gst);

    fprintf(fp,
            "Discount       : Rs. %.2f\n",
            order.discount);

    fprintf(fp,
            "Final Amount   : Rs. %.2f\n",
            order.finalAmount);

    fprintf(fp,
            "Payment        : %s\n",
            order.paymentMethod);

    fprintf(fp,
            "Status         : %s\n",
            order.status);

    fprintf(fp,
            "=============================================\n");

    fprintf(fp,
            "             THANK YOU!\n");

    fprintf(fp,
            "=============================================\n");

    fclose(fp);

    printf("\nReceipt saved successfully!\n");
}
