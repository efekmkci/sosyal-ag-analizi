#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Kullanýcý düðüm yapýsý
typedef struct UserNode {
    int id;
    struct UserNode *left, *right, *parent;
    char color;
    struct FollowerNode *following;
    struct FollowerNode *followers;
} UserNode;

// Takipçi düðüm yapýsý
typedef struct FollowerNode {
    int followerId;
    struct FollowerNode *next;
} FollowerNode;

UserNode *root = NULL;

// Yardýmcý fonksiyonlar (Kýrmýzý-Siyah Aðaç)
UserNode* createNode(int id) {
    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
    newNode->id = id;
    newNode->left = newNode->right = newNode->parent = NULL;
    newNode->color = 'R';
    newNode->following = newNode->followers = NULL;
    return newNode;
}

void rotateLeft(UserNode **root, UserNode *x) {
    UserNode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotateRight(UserNode **root, UserNode *y) {
    UserNode *x = y->left;
    y->left = x->right;
    if (x->right != NULL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL) *root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = y;
    x->right = y;
    y->parent = x;
}

void fixInsert(UserNode **root, UserNode *z) {
    while (z != *root && z->parent->color == 'R') {
        if (z->parent == z->parent->parent->left) {
            UserNode *y = z->parent->parent->right;
            if (y && y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(root, z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                rotateRight(root, z->parent->parent);
            }
        } else {
            UserNode *y = z->parent->parent->left;
            if (y && y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(root, z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                rotateLeft(root, z->parent->parent);
            }
        }
    }
    (*root)->color = 'B';
}

// Kullanýcý ekleme (Kýrmýzý-Siyah Aðaca)
void insertUser(int id) {
    UserNode *z = createNode(id);
    UserNode *y = NULL, *x = root;
    while (x != NULL) {
        y = x;
        if (z->id < x->id) x = x->left;
        else if (z->id > x->id) x = x->right;
        else {
            free(z);
            return;
        }
    }
    z->parent = y;
    if (y == NULL) root = z;
    else if (z->id < y->id) y->left = z;
    else y->right = z;
    fixInsert(&root, z);
}

// Kullanýcý bulma (Kýrmýzý-Siyah Aðaçta)
UserNode* searchUser(UserNode *root, int id) {
    if (root == NULL || root->id == id) return root;
    if (id < root->id) return searchUser(root->left, id);
    return searchUser(root->right, id);
}

// Takip ekleme (Graf yapýsýný güncelleme)
void addFollow(int followerId, int followeeId) {
    UserNode *follower = searchUser(root, followerId);
    UserNode *followee = searchUser(root, followeeId);

    if (!follower || !followee || followerId == followeeId) return;

    // Takip edilenlere ekle
    FollowerNode *newFollowing = (FollowerNode *)malloc(sizeof(FollowerNode));
    newFollowing->followerId = followeeId;
    newFollowing->next = follower->following;
    follower->following = newFollowing;

    // Takip edenlere ekle
    FollowerNode *newFollower = (FollowerNode *)malloc(sizeof(FollowerNode));
    newFollower->followerId = followerId;
    newFollower->next = followee->followers;
    followee->followers = newFollower;
}

// Dosyadan veri okuma (Mevcut veriseti.txt formatý için)
void loadData(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[256];
    int readingUsers = 0;
    int readingFriends = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *trimmedLine = strtok(line, "\r\n");
        if (trimmedLine == NULL) continue;

        if (strcmp(trimmedLine, "# Kullanicilar") == 0) {
            readingUsers = 1;
            readingFriends = 0;
            continue;
        } else if (strcmp(trimmedLine, "# Arkadaslik Iliskileri") == 0) {
            readingUsers = 0;
            readingFriends = 1;
            continue;
        }

        if (readingUsers) {
            if (strncmp(trimmedLine, "USER ", 5) == 0) {
                int userId = atoi(trimmedLine + 5);
                insertUser(userId);
            }
        } else if (readingFriends) {
            if (strncmp(trimmedLine, "FRIEND ", 7) == 0) {
                int followerId, followeeId;
                if (sscanf(trimmedLine + 7, "%d %d", &followerId, &followeeId) == 2) {
                    addFollow(followerId, followeeId);
                }
            }
        }
    }

    fclose(fp);
}

// Belirli mesafedeki arkadaþlarý bulma (DFS ile)
void findFriendsAtDistance(int startUserId, int distance, int currentDistance, int visited[]) {
    UserNode *currentUser = searchUser(root, startUserId);
    if (!currentUser || visited[currentUser->id]) return;

    visited[currentUser->id] = 1;

    if (currentDistance == distance) {
        printf("  Kullanici %d\n", currentUser->id);
        return;
    }

    FollowerNode *follower = currentUser->following;
    while (follower) {
        findFriendsAtDistance(follower->followerId, distance, currentDistance + 1, visited);
        follower = follower->next;
    }
}

void findFriends(int startUserId, int distance) {
    UserNode *startUser = searchUser(root, startUserId);
    if (!startUser) {
        printf("Kullanici %d bulunamadi.\n", startUserId);
        return;
    }
    printf("%d mesafedeki arkadaslar (%d):\n", distance, startUserId);
    int visited[1000] = {0};
    findFriendsAtDistance(startUserId, distance, 0, visited);
}

// Ortak arkadaþ analizi (takip edilenler üzerinden)
void findCommonFriends(int userId1, int userId2) {
    UserNode *user1 = searchUser(root, userId1);
    UserNode *user2 = searchUser(root, userId2);

    if (!user1 || !user2) {
        printf("Kullanicilardan biri veya ikisi bulunamadi.\n");
        return;
    }

    printf("%d ve %d kullanicilarinin ortak takip ettikleri:\n", userId1, userId2);
    FollowerNode *ptr1 = user1->following;
    while (ptr1) {
        UserNode *followedByUser1 = searchUser(root, ptr1->followerId);
        FollowerNode *ptr2 = user2->following;
        while (ptr2) {
            if (ptr1->followerId == ptr2->followerId) {
                printf("  Kullanici %d\n", ptr1->followerId);
            }
            ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
}

// Etki alaný hesabý (takip zinciri uzunluðu) - DFS ile
int calculateInfluenceDFS(int userId, int visited[]) {
    UserNode *user = searchUser(root, userId);
    if (!user || visited[user->id]) return 0;

    visited[user->id] = 1;
    int count = 1;
    FollowerNode *follower = user->following;
    while (follower) {
        count += calculateInfluenceDFS(follower->followerId, visited);
        follower = follower->next;
    }
    return count;
}

void calculateInfluence(int userId) {
    UserNode *user = searchUser(root, userId);
    if (!user) {
        printf("Kullanici %d bulunamadi.\n", userId);
        return;
    }
    int visited[1000] = {0};
    int influence = calculateInfluenceDFS(userId, visited) - 1;
    printf("Kullanici %d'in etki derecesi (takip edilen ag): %d\n", userId, influence);
}

// Topluluklarý bulma (Baðlantýlý Bileþenler)
void findCommunities() {
    if (root == NULL) {
        printf("Kullanici agaci bos.\n");
        return;
    }

    int visited[1000] = {0};
    int communityId = 1;

    UserNode *current = root;
    void traverseAndFind(UserNode *node) {
        if (node == NULL) return;

        if (!visited[node->id]) {
            printf("Topluluk %d: ", communityId++);
            int queue[1000];
            int head = 0, tail = 0;
            queue[tail++] = node->id;
            visited[node->id] = 1;

            while (head < tail) {
                int u = queue[head++];
                printf("%d ", u);
                UserNode *currentUser = searchUser(root, u);
                if (currentUser) {
                    FollowerNode *following = currentUser->following;
                    while (following) {
                        if (!visited[following->followerId]) {
                            visited[following->followerId] = 1;
                            queue[tail++] = following->followerId;
                        }
                        following = following->next;
                    }
                    FollowerNode *followers = currentUser->followers;
                    while (followers) {
                        if (!visited[followers->followerId]) {
                            visited[followers->followerId] = 1;
                            queue[tail++] = followers->followerId;
                        }
                        followers = followers->next;
                    }
                }
            }
            printf("\n");
        }
        traverseAndFind(node->left);
        traverseAndFind(node->right);
    }

    traverseAndFind(root);
}

int main() {
    setlocale(LC_ALL, "tr_TR.UTF-8");

    loadData("veriseti.txt");

    printf("\n--- Iliski Agaci (Kullanicilar) ---\n");
    printf("Kullanicilar kirmizi-siyah agacinda saklaniyor.\n");

    printf("\n--- Belirli Mesafedeki Arkadaslari Bulma ---\n");
    findFriends(101, 1);
    findFriends(102, 2);

    printf("\n--- Ortak Arkadas Analizi ---\n");
    findCommonFriends(101, 102);
    findCommonFriends(103, 104);

    printf("\n--- Etki Alani Hesabi ---\n");
    calculateInfluence(101);
    calculateInfluence(104);

    printf("\n--- Topluluk Tespiti ---\n");
    findCommunities();

    return 0;
}
