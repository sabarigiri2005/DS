#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[11];
    int weight;
} Package;

typedef struct {
    int package_count;
    int min_weight;
    int max_weight;
    Package* packages;
} PostOffice;

typedef struct {
    char name[11];
    int office_count;
    PostOffice* offices;
} Town;

void print_all_packages(Town town) {
    printf("%s:\n", town.name);
    for (int i = 0; i < town.office_count; i++) {
        printf("\t%d:\n", i);
        for (int j = 0; j < town.offices[i].package_count; j++) {
            printf("\t\t%s\n", town.offices[i].packages[j].id);
        }
    }
}

void send_all_acceptable_packages(Town* source, int source_office_index, Town* target, int target_office_index) {
    PostOffice* source_office = &source->offices[source_office_index];
    PostOffice* target_office = &target->offices[target_office_index];
    int target_min = target_office->min_weight;
    int target_max = target_office->max_weight;

    Package* new_packages = malloc(source_office->package_count * sizeof(Package));
    int new_count = 0;

    for (int i = 0; i < source_office->package_count; i++) {
        if (source_office->packages[i].weight >= target_min && source_office->packages[i].weight <= target_max) {
            target_office->packages = realloc(target_office->packages, (target_office->package_count + 1) * sizeof(Package));
            target_office->packages[target_office->package_count++] = source_office->packages[i];
        } else {
            new_packages[new_count++] = source_office->packages[i];
        }
    }

    free(source_office->packages);
    source_office->packages = new_packages;
    source_office->package_count = new_count;
}

Town town_with_most_packages(Town* towns, int town_count) {
    Town* max_town = &towns[0];
    int max_packages = 0;

    for (int i = 0; i < town_count; i++) {
        int current_packages = 0;
        for (int j = 0; j < towns[i].office_count; j++) {
            current_packages += towns[i].offices[j].package_count;
        }
        if (current_packages > max_packages) {
            max_packages = current_packages;
            max_town = &towns[i];
        }
    }
    return *max_town;
}

Town* find_town(Town* towns, int town_count, char* name) {
    for (int i = 0; i < town_count; i++) {
        if (strcmp(towns[i].name, name) == 0) {
            return &towns[i];
        }
    }
    return NULL;
}

int main() {
    int n;
    scanf("%d", &n);
    Town* towns = malloc(n * sizeof(Town));

    for (int i = 0; i < n; i++) {
        scanf("%s", towns[i].name);
        scanf("%d", &towns[i].office_count);
        towns[i].offices = malloc(towns[i].office_count * sizeof(PostOffice));
        for (int j = 0; j < towns[i].office_count; j++) {
            scanf("%d %d %d", &towns[i].offices[j].package_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
            towns[i].offices[j].packages = malloc(towns[i].offices[j].package_count * sizeof(Package));
            for (int k = 0; k < towns[i].offices[j].package_count; k++) {
                scanf("%s", towns[i].offices[j].packages[k].id);
                scanf("%d", &towns[i].offices[j].packages[k].weight);
            }
        }
    }

    int q;
    scanf("%d", &q);

    for (int i = 0; i < q; i++) {
        int query_type;
        scanf("%d", &query_type);

        if (query_type == 1) {
            char town_name[11];
            scanf("%s", town_name);
            Town* town = find_town(towns, n, town_name);
            print_all_packages(*town);
        } else if (query_type == 2) {
            char source_name[11], target_name[11];
            int source_index, target_index;
            scanf("%s %d %s %d", source_name, &source_index, target_name, &target_index);
            Town* source = find_town(towns, n, source_name);
            Town* target = find_town(towns, n, target_name);
            send_all_acceptable_packages(source, source_index, target, target_index);
        } else if (query_type == 3) {
            Town most_packages_town = town_with_most_packages(towns, n);
            printf("Town with the most number of packages is %s\n", most_packages_town.name);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < towns[i].office_count; j++) {
            free(towns[i].offices[j].packages);
        }
        free(towns[i].offices);
    }
    free(towns);

    return 0;
}
