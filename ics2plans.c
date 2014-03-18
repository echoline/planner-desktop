#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

void usage(char *prog) {
	fprintf(stderr, "usage:	%s filename.ics\n", prog);
	exit (-1);
}

int main(int argc, char **argv) {
	FILE *f;
	FILE *f2;
	unsigned char buf[1024];
	unsigned char summary[1024];
	unsigned char datevalue[1024];
	unsigned char tmp[16];
	char incal = 0, inevt = 0;
	int daybegin, monthbegin, yearbegin, dayend, monthend, yearend;
	char *home = getenv("HOME");
	unsigned char path[1024];
	struct stat st;

	if (argc < 2) {
		usage(argv[0]);
	}

	f = fopen(argv[1], "r");
	if (f == NULL)
		usage(argv[0]);

	while (fgets(buf, sizeof(buf), f)) {
		if (incal) {
			if (inevt) {
				if (!strncasecmp(buf, "END:VEVENT", 10)) {
					inevt = 0;
					snprintf(path, sizeof(path), "%s/.plans/", home);
					if (stat(path, &st))
						mkdir (path, 0700);

					snprintf(path, sizeof(path), "%s/.plans/%04d/", home, yearbegin);
					if (stat(path, &st))
						mkdir (path, 0700);

					snprintf(path, sizeof(path), "%s/.plans/%04d/%02d/", home, yearbegin, monthbegin);
					if (stat(path, &st))
						mkdir (path, 0700);

					snprintf(path, sizeof(path), "%s/.plans/%04d/%02d/%02d/", home, yearbegin, monthbegin, daybegin);
					if (stat(path, &st))
						mkdir (path, 0700);

					snprintf(path, sizeof(path), "%s/.plans/%04d/%02d/%02d/index.txt", home, yearbegin, monthbegin, daybegin);
					if (!stat(path, &st)) {
						f2 = fopen(path, "a");
						if (f2 == NULL)
							exit (-2);
					} else {
						f2 = fopen(path, "w");
						if (f2 == NULL)
							exit(-2);

						fprintf(f2, "\n");
					}

					fprintf(f2, "\n");
					fprintf(f2, "%s\n", summary);
					fclose(f2);

					continue;
				}

				if (!strncasecmp(buf, "SUMMARY:", 8)) {
					memcpy(summary, &buf[8], strlen(buf)-8);
					summary[strcspn(summary, "\r\n")] = '\0';
					continue;
				}

				if (!strncasecmp(buf, "DTSTART;VALUE=DATE:", 19)) {
					memcpy(datevalue, &buf[19], strlen(buf)-19);
					datevalue[strcspn(datevalue, "\r\n")] = '\0';
					memcpy(tmp, datevalue, 4);
					tmp[4] = '\0';
					yearbegin = atoi(tmp);

					memcpy(tmp, &datevalue[4], 2);
					tmp[2] = '\0';
					monthbegin = atoi(tmp);

					memcpy(tmp, &datevalue[6], 2);
					tmp[2] = '\0';
					daybegin = atoi(tmp);
					continue;
				}

				if (!strncasecmp(buf, "DTEND;VALUE=DATE:", 17)) {
					memcpy(datevalue, &buf[17], strlen(buf)-17);
					datevalue[strcspn(datevalue, "\r\n")] = '\0';
					memcpy(tmp, datevalue, 4);
					tmp[4] = '\0';
					yearend = atoi(tmp);

					memcpy(tmp, &datevalue[4], 2);
					tmp[2] = '\0';
					monthend = atoi(tmp);

					memcpy(tmp, &datevalue[6], 2);
					tmp[2] = '\0';
					dayend = atoi(tmp);
					continue;
				}
			}

			if (!strncasecmp(buf, "END:VCALENDAR", 13)) {
				incal = 0;
				continue;
			}

			if (!strncasecmp(buf, "BEGIN:VEVENT", 12)) {
				inevt = 1;
				continue;
			}
		}

		if (!strncasecmp(buf, "BEGIN:VCALENDAR", 15)) {
			incal = 1;
			continue;
		}
	}
}
