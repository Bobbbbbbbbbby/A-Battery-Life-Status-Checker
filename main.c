#include <stdio.h>
#include <unistd.h>

#define FULL_ENERGY_PATH \
    "/sys/class/power_supply/BAT0/energy_full"

#define NOW_ENERGY_PATH \
    "/sys/class/power_supply/BAT0/energy_now"


#define NOW_STATUS \
    "/sys/class/power_supply/BAT0/status"

int get_energy_full();
int get_energy_now();

/*
* @return
* 0 is Discharging, 1 is charging
*/
int get_battery_status();

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Start calculating battery life!");
    int energy_start = get_energy_now();
    int time = 0;
    while(1)
    {
        sleep(10);
        int status_now = get_battery_status();
        int energy_now = get_energy_now();
        int energy_consumption = energy_start - energy_now;
        time += 10;
        double avg_spd = (double)energy_consumption / (double)time;

        double avg_time_left = (double)energy_now / avg_spd;
        int hr_count = 0;
        while(avg_time_left > 3600)
        {
            hr_count++;
            avg_time_left -= 3600;
        }
        int min_count = 0;
        while(avg_time_left > 60)
        {
            min_count++;
            avg_time_left -= 60;
        }
        printf("\r                                    \r");
        printf("%d : %d Remaining", hr_count, min_count);
    }
    return 0;
}

int get_energy_full()
{
    int ret;
    FILE* fd;
    fd = fopen(FULL_ENERGY_PATH, "r");
    fscanf(fd, "%d", &ret);
    fclose(fd);
    return ret;
}

int get_energy_now()
{
    int ret;
    FILE* fd;
    fd = fopen(NOW_ENERGY_PATH, "r");
    fscanf(fd, "%d", &ret);
    fclose(fd);
    return ret;
}

int get_battery_status()
{
    int ret;
    FILE* fd;
    fd = fopen(NOW_STATUS, "r");
    char buf[20];
    fscanf(fd, "%s", buf);
    fclose(fd);
    if(buf[0] == 'D')
        return 0;
    else
        return 1;
}