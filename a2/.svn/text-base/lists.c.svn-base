#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
/* Create a group and initialize all its attributes
*/
Group *create_group(const char *group_name) {
    // create a new group and check if it is malloced successfully
    Group *new_group = malloc( sizeof(Group));
    if (new_group == NULL) {
        fprintf(stderr, "Error: %s\n", "malloc failed");
        exit(EXIT_FAILURE);
    } // also malloc for the name and check it
    new_group->name = malloc( strlen(group_name) + 1);
    if (new_group->name == NULL) {
        fprintf(stderr, "Error: %s\n", "malloc failed");
        exit(EXIT_FAILURE);
    } // initialize the group
    strcpy(new_group->name, group_name);
    new_group->users = NULL;
    new_group->xcts = NULL;
    new_group->next = NULL;
    return new_group;
}

/* Create a user and initialize all its attributes
*/
User *create_user(const char *user_name) {
    // create a new group and check if it is malloced successfully
    User *new_user = malloc( sizeof(User));
    if (new_user == NULL) {
        fprintf(stderr, "Error: %s\n", "malloc failed");
        exit(EXIT_FAILURE);
    } // also malloc for the name and check it
    new_user->name = malloc( strlen(user_name) + 1);
    if (new_user->name == NULL) {
        fprintf(stderr, "Error: %s\n", "malloc failed");
        exit(EXIT_FAILURE);
    } // initialize the user
    strcpy(new_user->name, user_name);
    new_user->balance = 0.0;
    new_user->next = NULL;
    return new_user;
}

/* Create a transaction and initialize all its attributes
*/
Xct *create_xct(User *user, double amount) {
    // create a new group and check if it is malloced successfully
    Xct *new_xct = malloc( sizeof(Xct));
    if (new_xct == NULL) {
        fprintf(stderr, "Error: %s\n", "malloc failed");
        exit(EXIT_FAILURE);
    } // initialize the user
    // link the name to the user's name, since every xct shares the same
    new_xct->name = user->name; 
    new_xct->amount = amount;
    new_xct->next = NULL;
    return new_xct;
}

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    // create new group and initialize with a copy of group_name
    Group *new_group = create_group(group_name);

    int result = -1;
    // insert to head if it is a empty list
    if (*group_list_ptr == NULL) {
        *group_list_ptr = new_group;
        result = 0;
    // define current node and find the end of the list
    } else {
        Group *current_group = *group_list_ptr;
        // loop through the list to check if theres duplicated name and move current toward the end
        while ((current_group->next != NULL) && (strcmp(current_group->name, group_name) != 0)){
            current_group = current_group->next;
        }
        // add the new node to the end of the list
        if (strcmp(current_group->name, group_name) != 0) {
            current_group->next = new_group;
        result = 0;
        }
        
    }
    return result;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
    // create a pointer as the current node
    Group *current_group = group_list;
    // loop through the list and print every node passed and move current forward
    while (current_group != NULL) {
        printf("%s\n", current_group->name);
        current_group = current_group->next;
    }
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    // create a current node
    Group *current_group = group_list;
    // loop through the list 
    while (current_group != NULL) {
        // if the name matches, return the current group
        if (strcmp(current_group->name, group_name) == 0) {
            return current_group;
        }
        // update current forward
        current_group = current_group->next;
    }
    return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    // check if the user already exist
    User *user = find_prev_user(group, user_name);
    // if it does not exist, create and add it to the frond
    if (user == NULL) {
        User *new_user = create_user(user_name);
        new_user->next = group->users;
        group->users = new_user;
        return 0;
    }
    return -1;
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    // find the previous user of the removing user
    User *prev_user = find_prev_user(group, user_name);
    int result = -1;
    User *user;
    // if the user exist in the list
    if (prev_user != NULL) {
        // if the user is the first one, update the head
        if (strcmp(group->users->name, user_name) == 0) {
            user = group->users;
            group->users = group->users->next;
        // remove the user by relinking the list 
        } else {
            user = prev_user->next;
            prev_user->next = prev_user->next->next;
        }
        // also remove all the transactions of the user and free their memory
        remove_xct(group, user->name);
        free (user->name);
        free (user);
        result = 0;
    }
    return result;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
    // create a current user 
    User *current_user = group->users;
    // loop through the list and print every node passed and move current forward
    while (current_user != NULL) {
        printf("%s\n", current_user->name);
        current_user = current_user->next;
    }
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    // find the position of the user
    User *prev_user = find_prev_user(group, user_name);
    int result = -1;
    // if the user exist
    if (prev_user != NULL) { // if the user is the first one
        if (strcmp(group->users->name, user_name) == 0) {
            printf("%g\n", prev_user->balance);
        } else {
            printf("%g\n", prev_user->next->balance);
        }
        result = 0;
    }
    return result;
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
    User *current_user = group->users;
    int result = -1;
    // loop through the list and check is current's balance is the same as the head's balance
    while ((current_user != NULL) && (group->users->balance == current_user->balance)) {
        printf("%s\n", current_user->name); // print and move current forward
        current_user = current_user->next;
        result = 0;
    }
    return result;
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    User *current_user = group->users;
    User *previous_user;
    // if the list is not empty
    if (current_user != NULL) {
        // if the user is at the first one
        if (strcmp(current_user->name, user_name) == 0) {
            return current_user;
        } // loop through the list and also keep track of the previous user
        while ((current_user != NULL) && (strcmp(current_user->name, user_name) != 0)) {
            previous_user = current_user;
            current_user = current_user->next;
        }
        // if the user is found, return the previous user
        if (current_user != NULL) {
            return previous_user;
        }
    }
    // if user is not found, return NULL
    return NULL;
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    User *prev_user = find_prev_user(group, user_name);
    User *user;
    User *curr_user;
    int result = -1;
    // if the user is found in the user list
    if (prev_user != NULL) { 
        Xct *new_xct; 
        // if the user is the first one, remove the user and update the head
        if (strcmp(group->users->name, user_name) == 0) {
            new_xct = create_xct(prev_user, amount);
            user = prev_user;
            group->users = prev_user->next;
            curr_user = prev_user->next;
            prev_user = NULL; // set prev pointer to NULL since curr is head
        } else { // remove the user by relinking the list
            new_xct = create_xct(prev_user->next, amount);
            user = prev_user->next;
            prev_user->next = prev_user->next->next;
            curr_user = prev_user->next;
        } // add the balance
        user->balance += amount;
        // move current user to the right until reaching the correct position to insert
        while ((curr_user != NULL) && (user->balance > curr_user->balance)) {
            prev_user = curr_user;
            curr_user = curr_user->next;
        } // link user to the current (first user with balance greater than him)
        user->next = curr_user;
        // if prev user is still NULL, just update the head
        if (prev_user == NULL) {
            group->users = user;
        // insert the user by linking prev_user to it
        } else {
            prev_user->next = user;
        }
        // add the new transaction into the list
        new_xct->next = group->xcts;
        group->xcts = new_xct;
        result = 0;
    }

    return result;
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long num_xct) {
    Xct *current_xct = group->xcts;
    long count = 0;
    // loop through the list and check if count is less than num_xut or reaches NULL
    while ((current_xct != NULL) && (count < num_xct)) {
        printf("%s %.2f\n", current_xct->name, current_xct->amount);
        count += 1; // print and increase count
        current_xct = current_xct->next;
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
    Xct *current_xct = group->xcts;
    Xct *previous_xct = NULL;
    Xct *xct;
    while (current_xct != NULL) { // find all the xct with given user_name
        if (strcmp(current_xct->name, user_name) == 0){
            xct = current_xct; // store xct to free later
            if (previous_xct == NULL) { // if the xct is the frist one, update the head
                group->xcts = current_xct->next;
            } else { // xct is not the first one, relink the list
                previous_xct->next = current_xct->next;
            } // update current before freeing the xct
            current_xct = current_xct->next; 
            free(xct);
        } else { // if user_name is not the same, update both pointers
            previous_xct = current_xct;
            current_xct = current_xct->next;
        } 
    }
}
