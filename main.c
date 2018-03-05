#include "usermgr.h"
#include "ioctrl.h"
#include "serializer.h"
#include "stdafx.h"
#include "zhwkre/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void user_menu(User *user) {
    int op = 1;
    while (op) {
        printf("\n\n");
        printf("             ");
        printf(" -> Current User: %s <-\n",user->username);
        printf("-------------------------------------------------\n");
        printf("    	  1. ShowFriends             \n");
        printf("    	  2. ShowWatchers          \n");
        printf("    	  3. ShowWatchings             \n");
        printf("    	  4. AddFriend                \n");
        printf("    	  5. AddWacher              \n");
        printf("    	  6. AddWatching          \n");
        printf("    	  7. DeleteFriend          \n");
        printf("    	  8. DeleteWatcher    \n");
        printf("    	  9. DeleteWatching \n");
        printf("    	  0. BackToMainMenu    \n");
        printf("-------------------------------------------------\n");
        printf("       Please choose your operation[0~10]:");
        scanf("%d",&op);
        int i = 0;
        switch (op) {
            case 1: {
                forset(user->friends,it){
                    printf("%s\n",qSetIterator_deref(it));
                }
            }
            case 2: {
                forset(user->watcher,it){
                    printf("%s\n",qSetIterator_deref(it));
                }
            }
            case 3: {
                forset(user->watching,it){
                    printf("%s\n",qSetIterator_deref(it));
                }
            }
            case 4: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                if(qBTree__ptr_at(users,NULL,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // add
                qSet__insert(&(user->friends),destname,USERNAMELEN);
            }
            case 5: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                if(qBTree__ptr_at(users,NULL,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // add
                qSet__insert(&(user->watcher),destname,USERNAMELEN);
            }
            case 6: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                if(qBTree__ptr_at(users,NULL,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // add
                qSet__insert(&(user->watching),destname,USERNAMELEN);
            }
            case 7: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                qSetIterator tmpiter;
                if(qSet__ptr_at(user->friends,&tmpiter,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // add
                qSet__erase(&(user->friends),tmpiter);
            }
            case 8: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                qSetIterator tmpiter;
                if(qSet__ptr_at(user->watcher,&tmpiter,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // add
                qSet__erase(&(user->watcher),tmpiter);
            }
            case 9: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                qSetIterator tmpiter;
                if(qSet__ptr_at(user->watching,&tmpiter,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // add
                qSet__erase(&(user->watching),tmpiter);
            }
            case 0:
                break;
        }

    }
}

int main() {
    Usermgr_init();
    int op = 1;
    while (op) {
        printf("\n\n");
        printf("             Set Operations               \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. Load           \n");
        printf("    	  2. Save           \n");
        printf("    	  3. Randomize          \n");
        printf("    	  4. ListAllUsers             \n");
        printf("    	  5. AddUser                 \n");
        printf("    	  6. RemoveUser              \n");
        printf("    	  7. EditUser          \n");
        printf("    	  8. ShowCommonFriends          \n");
        printf("    	  9. ShowCommonFollowings    \n");
        printf("    	 10. ShowSecondDegreeFriends \n");
        printf("    	 11. BatchReset \n");
        printf("    	  0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("       Please choose your operation[0~10]:");
        scanf("%d",&op);
        int i = 0;
        switch (op) {
            case 1: {
                printf("Please input filename (255)> ");
                char filename[256];
                memset(filename,0,256);
                scanf("%s",filename);
                if(read_users_from_file(filename)){
                    qLogFail("Read failed. Avoid corrupted global structures, please restart program.");
                }
            }
            case 2: {
                printf("Please input filename (255)> ");
                char filename[256];
                memset(filename,0,256);
                scanf("%s",filename);
                int bytes = save_users_to_file(filename);
                qLogSuccfmt("%.2lf KB written.",((double)bytes)/1024.0f);
            }
            case 3:
                printf("Started generating randomized dataset.\n");
                printf("The time needed depends on your computer & dataset size.\n");
                Usermgr_randomize(1999);
            case 4: {
                formap(users,it){
                    printf("%s\n",qBTreeIterator_deref(it).key);
                }
            }
            case 5: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // init user
                User user=User_constructor();
                memcpy(user.username,destname,USERNAMELEN);
                // add
                qBTree__insert(&users,destname,USERNAMELEN,&user,sizeof(User));
            }
            case 6: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                // check Exist
                qBTreeIterator it;
                if(qBTree__ptr_at(users,&it,destname,USERNAMELEN)){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                // erase
                qBTree__erase(&users,it);
            }
            case 7: {
                printf("Please input name(%d):",USERNAMELEN-1);
                char destname[USERNAMELEN];
                int pos = fscanf(stdin,destname,USERNAMELEN-1);
                destname[pos] = '\0';
                User *user = Usermgr_getuser(destname);
                if(user == NULL){
                    printf("User %s not exist.\n",destname);
                    continue;
                }
                user_menu(user);
            }
            case 8: {
                printf("Please input first person(%d):",USERNAMELEN-1);
                char destnamea[USERNAMELEN];
                int pos = fscanf(stdin,destnamea,USERNAMELEN-1);
                destnamea[pos] = '\0';
                User *usera = Usermgr_getuser(destnamea);
                if(usera == NULL){
                    printf("user %s not exist.\n",destnamea);
                    continue;
                }
                printf("Please input second person(%d):",USERNAMELEN-1);
                char destnameb[USERNAMELEN];
                pos = fscanf(stdin,destnameb,USERNAMELEN-1);
                destnameb[pos] = '\0';
                User *userb = Usermgr_getuser(destnameb);
                if(userb == NULL){
                    printf("user %s not exist.\n",destnameb);
                    continue;
                }
                qSetDescriptor desc = Usermgr_bothfriend(*usera,*userb);
                forset(desc,it){
                    printf("%s\n",qSetIterator_deref(it));
                }
                qSet_destructor(desc);
            }
            case 9: {
                printf("Please input first person(%d):",USERNAMELEN-1);
                char destnamea[USERNAMELEN];
                int pos = fscanf(stdin,destnamea,USERNAMELEN-1);
                destnamea[pos] = '\0';
                User *usera = Usermgr_getuser(destnamea);
                if(usera == NULL){
                    printf("user %s not exist.\n",destnamea);
                    continue;
                }
                printf("Please input second person(%d):",USERNAMELEN-1);
                char destnameb[USERNAMELEN];
                pos = fscanf(stdin,destnameb,USERNAMELEN-1);
                destnameb[pos] = '\0';
                User *userb = Usermgr_getuser(destnameb);
                if(userb == NULL){
                    printf("user %s not exist.\n",destnameb);
                    continue;
                }
                qSetDescriptor desc = Usermgr_bothwatch(*usera,*userb);
                forset(desc,it){
                    printf("%s\n",qSetIterator_deref(it));
                }
                qSet_destructor(desc);
            }
            case 10: {
                printf("Please input first person(%d):",USERNAMELEN-1);
                char destnamea[USERNAMELEN];
                int pos = fscanf(stdin,destnamea,USERNAMELEN-1);
                destnamea[pos] = '\0';
                User *usera = Usermgr_getuser(destnamea);
                if(usera == NULL){
                    printf("user %s not exist.\n",destnamea);
                    continue;
                }
                qSetDescriptor desc = Usermgr_2ndfriend(*usera);
                forset(desc,it){
                    printf("%s\n",qSetIterator_deref(it));
                }
                qSet_destructor(desc);
            }
            case 11: {
                printf("Please input filename (255)> ");
                char filename[256];
                memset(filename,0,256);
                scanf("%s",filename);
                FILE* fp = fopen(filename,"r");
                if(fp == NULL){
                    qLogFailfmt("Failed to open file %s.",filename);
                    continue;
                }
                int counter = 0;
                while(!feof(fp)){
                    qBinarySafeString thisline = readline(fp);
                    User usr=User_constructor();
                    memcpy(usr.username,thisline.str,strlen(thisline.str));
                    Usermgr_adduser(usr);
                    counter++;
                }
                qLogSuccfmt("Added %d users.",counter);
            }
            case 0:
                break;
        }
    }

}