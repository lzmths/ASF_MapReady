#include "foot_print.h"

#define VERSION 1.0

void usage()
{
  printf("\n"
   "USAGE:\n"
   "   foot_print <granule table> <satellite> <beam mode>\n");
  printf("\n"
   "REQUIRED ARGUMENTS:\n"
   "   granule table             database dump of the table\n"
   "   satellite                 satellite name (E1, E2, J1, R1, A3)\n"
   "   beam mode                 R1 - FN1 to FN5, ST1 to ST7, SNA, SNB,\n"
   "                                  SWA, SWB, WD1 to WD3, EL1, EH3, EH4, EH6\n"
   "                             E1 - A, B, C, D, E, F, G (phases)\n"
   "                             E2 - STD\n"
   "                             J1 - STD\n"
   "                             A3 - FBS, FBD, PLR, WB1, WB2\n");
  printf("\n"
   "DESCRIPTION:\n"
   "   This program converts information for image stacks out of database into\n"
   "   ArcGIS shape files.\n");
  printf("\n"
   "Version %.2f, ASF SAR Tools\n"
   "\n",VERSION);
  exit(EXIT_FAILURE);
}

static int is_granule(char *line, char *satellite, char *beam_mode,
		      			double off_nadir)
{
  int orbit, ret = FALSE;
  char sat[5], mode[10];
  double angle;
  
  if (strstr(line, "GRANULE_NAME"))
  	ret = FALSE;
  else {
		sprintf(sat, "%s", get_str(line, 1));
  	sprintf(mode, "%s", get_str(line, 2));
  	if (strcmp_case(satellite, "A3") == 0) {
	  	angle = get_double(line, 3);
    	if (strcmp_case(satellite, sat) == 0 && 
    			strcmp_case(beam_mode, mode) == 0 &&
					FLOAT_EQUIVALENT(off_nadir, angle))
      	ret = TRUE;
  	}
  	else if (strcmp_case(satellite, "E1") == 0) {
    	orbit = get_int(line, 3);
    	if (orbit >= E1_A_MIN_ORBIT && orbit <= E1_A_MAX_ORBIT)
      	strcpy(mode, "A");
    	else if (orbit >= E1_B_MIN_ORBIT && orbit <= E1_B_MAX_ORBIT)
      	strcpy(mode, "B");
    	else if (orbit >= E1_C_MIN_ORBIT && orbit <= E1_C_MAX_ORBIT)
      	strcpy(mode, "C");
    	else if (orbit >= E1_D_MIN_ORBIT && orbit <= E1_D_MAX_ORBIT)
      	strcpy(mode, "D");
    	else if (orbit >= E1_E_MIN_ORBIT && orbit <= E1_E_MAX_ORBIT)
      	strcpy(mode, "E");
    	else if (orbit >= E1_F_MIN_ORBIT && orbit <= E1_F_MAX_ORBIT)
      	strcpy(mode, "F");
    	else if (orbit >= E1_G_MIN_ORBIT && orbit <= E1_G_MAX_ORBIT)
      	strcpy(mode, "G");
    	if (strcmp_case(satellite, sat) == 0 && 
    			strcmp_case(beam_mode, mode) == 0)
      	ret = TRUE;
  	}
  	else if (strcmp_case(satellite, "AS") == 0) {
  		if (strcmp_case(satellite, sat) == 0 &&
  				(strcmp_case(mode, "3FP") == 0 ||
  				 strcmp_case(mode, "ATI") == 0 ||
  				 strcmp_case(mode, "XTI") == 0))
  			ret = TRUE;
  	}
  	else if (strcmp_case(satellite, "UA") == 0 ||
  					 strcmp_case(satellite, "AI") == 0) {
  		if (strcmp_case(satellite, sat) == 0 &&
  				(strcmp_case(mode, "POL") == 0 ||
  				 strcmp_case(mode, "RPI") == 0))
  			ret = TRUE;
  	}
  	else {
    	if (strcmp_case(satellite, sat) == 0 && 
    			strcmp_case(beam_mode, mode) == 0)
      	ret = TRUE;
  	}
  }
    
  return ret;
}

static int line2granule(char *line, char *satellite, char *beam_mode,
			double off_nadir, granule_t *granule)
{
  int ret = TRUE;
  granule_t g;

  // Table column names
  // GRANULE_NAME - name
  // PLATFORM_TYPE - satellite
  // BEAM_MODE_TYPE - beam mode
  // OFF_NADIR_ANGLE - off nadir angle
  // REVOLUTION - orbit
  // FRAME_ID - frame
  // START_TIME - acquisition start date
  // END_TIME - acquisition end date
  // ASCENDING_DESCENDING - orbit direction
  // PATH_NUMBER - path
  // NEAR_START_LAT - near start latitude
  // NEAR_START_LON - near start longitude
  // FAR_START_LAT - far start latitude
  // FAR_START_LON - far start longitude
  // FAR_END_LAT - far end latitude
  // FAR_END_LON - far end longitude
  // NEAR_END_LAT - near end latitude
  // NEAR_END_LON - near end longitude

  // Read the data from CSV file
  if (strcmp_case(satellite, "E1") == 0 ||
  	  strcmp_case(satellite, "E2") == 0 ||
  	  strcmp_case(satellite, "J1") == 0 ||
  	  strcmp_case(satellite, "R1") == 0 ||
  	  strcmp_case(satellite, "SS") == 0) {
    strcpy(g.name, get_str(line, 0));
    strcpy(g.satellite, get_str(line, 1));
    strcpy(g.beam_mode, get_str(line, 2));
  	g.off_nadir = -1.0;
    g.orbit = get_int(line, 3);
  	g.frame = get_int(line, 4);
  	strcpy(g.start_date, get_str(line, 5));
  	strcpy(g.end_date, get_str(line, 6));
  	strcpy(g.orbit_dir, get_str(line, 7));
  	g.path = -1;
  	g.near_start_lat = get_double(line, 8);
  	g.near_start_lon = get_double(line, 9);
  	g.near_end_lat = get_double(line, 10);
  	g.near_end_lon = get_double(line, 11);
  	g.far_start_lat = get_double(line, 12);
  	g.far_start_lon = get_double(line, 13);
  	g.far_end_lat = get_double(line, 14);
  	g.far_end_lon = get_double(line, 15);
  	if (strcmp_case(g.satellite, "R1") == 0 &&
  			(strcmp_case(g.beam_mode, "SNA") == 0 ||
  			 strcmp_case(g.beam_mode, "SNB") == 0 ||
  			 strcmp_case(g.beam_mode, "SWA") == 0 ||
  			 strcmp_case(g.beam_mode, "SWB") == 0))
  		g.insar = 0;
  	else
  		g.insar = 1;
  }
  else if (strcmp_case(satellite, "A3") == 0) {
    strcpy(g.name, get_str(line, 0));
    strcpy(g.satellite, get_str(line, 1));
    strcpy(g.beam_mode, get_str(line, 2));
    g.off_nadir = get_double(line, 3);
    g.orbit = get_int(line, 4);
  	g.frame = get_int(line, 5);
  	strcpy(g.start_date, get_str(line, 6));
  	strcpy(g.end_date, get_str(line, 7));
  	strcpy(g.orbit_dir, get_str(line, 8));
  	g.path = get_int(line, 9);
  	g.near_start_lat = get_double(line, 10);
  	g.near_start_lon = get_double(line, 11);
  	g.near_end_lat = get_double(line, 12);
  	g.near_end_lon = get_double(line, 13);
  	g.far_start_lat = get_double(line, 14);
  	g.far_start_lon = get_double(line, 15);
  	g.far_end_lat = get_double(line, 16);
  	g.far_end_lon = get_double(line, 17);
  	if (strcmp_case(g.beam_mode, "WB1") == 0 ||
  			strcmp_case(g.beam_mode, "WB2") == 0)
  		g.insar = 0;
  	else
  		g.insar = 1;
  }
  else {
    strcpy(g.name, get_str(line, 0));
    strcpy(g.satellite, get_str(line, 1));
    strcpy(g.beam_mode, get_str(line, 2));
  	g.off_nadir = -1.0;
  	g.orbit = -1;
  	g.frame = -1;
  	strcpy(g.start_date, get_str(line, 5));
  	strcpy(g.end_date, get_str(line, 6));
  	strcpy(g.orbit_dir, get_str(line, 7));
  	g.path = -1;
  	g.near_start_lat = get_double(line, 8);
  	g.near_start_lon = get_double(line, 9);
  	g.near_end_lat = get_double(line, 10);
  	g.near_end_lon = get_double(line, 11);
  	g.far_start_lat = get_double(line, 12);
  	g.far_start_lon = get_double(line, 13);
  	g.far_end_lat = get_double(line, 14);
  	g.far_end_lon = get_double(line, 15);
  	g.insar = 0;
  }

  if (strcmp_case(g.satellite, "E1") == 0) {
    if (g.orbit >= E1_A_MIN_ORBIT && g.orbit <= E1_A_MAX_ORBIT)
      strcpy(g.beam_mode, "A");
    else if (g.orbit >= E1_B_MIN_ORBIT && g.orbit <= E1_B_MAX_ORBIT)
      strcpy(g.beam_mode, "B");
    else if (g.orbit >= E1_C_MIN_ORBIT && g.orbit <= E1_C_MAX_ORBIT)
      strcpy(g.beam_mode, "C");
    else if (g.orbit >= E1_D_MIN_ORBIT && g.orbit <= E1_D_MAX_ORBIT)
      strcpy(g.beam_mode, "D");
    else if (g.orbit >= E1_E_MIN_ORBIT && g.orbit <= E1_E_MAX_ORBIT)
      strcpy(g.beam_mode, "E");
    else if (g.orbit >= E1_F_MIN_ORBIT && g.orbit <= E1_F_MAX_ORBIT)
      strcpy(g.beam_mode, "F");
    else if (g.orbit >= E1_G_MIN_ORBIT && g.orbit <= E1_G_MAX_ORBIT)
      strcpy(g.beam_mode, "G");
  }
  *granule = g;
  
  return ret;
}

static void my_swap(granule_t *a, granule_t *b)
{
  granule_t c=*a;
  *a = *b;
  *b = c;
}

static void my_sort(granule_t *list, int left, int right)
{
  register int i, j;
  granule_t x;
  
  i = left;
  j = right;
  x = list[(left+right)/2];
  
  do {
    while ((strcmp_case(list[i].name, x.name) < 0) && (i < right))
      i++;
    while ((strcmp_case(list[j].name, x.name) > 0) && (j > left))
      j--;
    if (i <= j) {
      my_swap(&list[i], &list[j]);
      i++;
      j--;
    }
  } while (i <= j);
  
  if (left < j) 
    my_sort(list, left, j);
  if (i < right)
    my_sort(list, i, right);
}

int main(int argc, char **argv)
{
  long granule_count;
  char granule_table[512], line[1024];
  char satellite[15], beam[10], beam_mode[5];

  // Parse command line
  if ((argc-currArg)<3) {
    printf("Insufficient arguments.\n"); 
    usage(argv[0]);
  }
  strcpy(granule_table, argv[currArg]);
  strcpy(satellite, argv[currArg+1]);
  strcpy(beam_mode, argv[currArg+2]);
  
  asfSplashScreen(argc, argv);

  FILE *fp, *fpGran;
  char outFile[255], granuleFile[255];
  int ii, kk, ll, mm, orbit, track, track_count, frame_count, max_frame_count;
  int inc, stack_id, beam_mode_count;
  long nn;
  double off_nadir;

  if (strcmp(satellite, "R1") == 0) {
    track_count = R1_ORBITS_PER_CYCLE;
    max_frame_count = 900;
    beam_mode_count = 1;
    inc = 1;
  }
  else if (strcmp(satellite, "E1") == 0) {
    if (strcmp_case(beam_mode, "A") == 0)
      track_count = E1_A_ORBITS_PER_CYCLE;
    else if (strcmp_case(beam_mode, "B") == 0)
      track_count = E1_B_ORBITS_PER_CYCLE;
    else if (strcmp_case(beam_mode, "C") == 0)
      track_count = E1_C_ORBITS_PER_CYCLE;
    else if (strcmp_case(beam_mode, "D") == 0)
      track_count = E1_D_ORBITS_PER_CYCLE;
    else if (strcmp_case(beam_mode, "E") == 0)
      track_count = E1_E_ORBITS_PER_CYCLE;
    else if (strcmp_case(beam_mode, "F") == 0)
      track_count = E1_F_ORBITS_PER_CYCLE;
    else if (strcmp_case(beam_mode, "G") == 0)
      track_count = E1_G_ORBITS_PER_CYCLE;
    max_frame_count = 900;
    beam_mode_count = 1;
    inc = 1;
  }
  else if (strcmp(satellite, "E2") == 0) {
    track_count = E2_ORBITS_PER_CYCLE;
    max_frame_count = 900;
    beam_mode_count = 1;
    inc = 1;
  }
  else if (strcmp(satellite, "J1") == 0) {
    track_count = J1_ORBITS_PER_CYCLE;
    max_frame_count = 900;
    beam_mode_count = 1;
    inc = 1;
  }
  else if (strcmp(satellite, "A3") == 0) {
    track_count = PSR_ORBITS_PER_CYCLE;
    max_frame_count = 7200;
    if (strcmp_case(beam_mode, "FBS") == 0)
      beam_mode_count = FBS_COUNT;
    else if (strcmp_case(beam_mode, "FBD") == 0)
      beam_mode_count = FBD_COUNT;
    else if (strcmp_case(beam_mode, "PLR") == 0)
      beam_mode_count = PLR_COUNT;
    else if (strcmp_case(beam_mode, "WB1") == 0)
      beam_mode_count = WB1_COUNT;
    else if (strcmp_case(beam_mode, "WB2") == 0)
      beam_mode_count = WB2_COUNT;
    if (strncmp_case(beam_mode, "WB", 2) == 0)
      inc = 50;
    else
      inc = 10;
  }
  else {
  	track_count = 1;
  	max_frame_count = 1;
  	beam_mode_count = 1;
  }

  // Assign CSV file name
  sprintf(granuleFile, "%s_%s_granules.csv", satellite, beam_mode);
  fpGran = FOPEN(granuleFile, "w");
	fprintf(fpGran, "Stack ID,Granule,Satellite,Beam Mode,Off Nadir,Orbit,"
	  "Frame,Start Date,End Date,Direction,Path,InSAR,Lat1,Lon1,Lat2,Lon2,"
	  "Lat3,Lon3,Lat4,Lon4\n");

  stack_id = 0;

  for (ii=0; ii<beam_mode_count; ii++) {

    if (strcmp_case(satellite, "A3") == 0) {
      if (strcmp_case(beam_mode, "FBS") == 0) {
				sprintf(beam, "%s %.1lf", beam_mode, fbs_modes[ii]);
				off_nadir = fbs_modes[ii];
      }
      else if (strcmp_case(beam_mode, "FBD") == 0) {
				sprintf(beam, "%s %.1lf", beam_mode, fbd_modes[ii]);
				off_nadir = fbd_modes[ii];
      }
      else if (strcmp_case(beam_mode, "PLR") == 0) {
				sprintf(beam, "%s %.1lf", beam_mode, plr_modes[ii]);
				off_nadir = plr_modes[ii];
      }
      else if (strcmp_case(beam_mode, "WB1") == 0) {
				sprintf(beam, "%s %.1lf", beam_mode, wb1_modes[ii]);
				off_nadir = wb1_modes[ii];
      }
      else if (strcmp_case(beam_mode, "WB2") == 0) {
				sprintf(beam, "%s %.1lf", beam_mode, wb2_modes[ii]);
				off_nadir = wb2_modes[ii];
      }
      asfPrintStatus("\nProcessing ALOS %s granules ...\n", beam);
    }
    else if (strcmp_case(satellite, "R1") == 0)
      asfPrintStatus("\nProcessing R1 %s granules ...\n", beam_mode);
    else if (strcmp_case(satellite, "E1") == 0)
      asfPrintStatus("\nProcessing E1 granules (phase %s) ...\n", beam_mode);
    else if (strcmp_case(satellite, "E2") == 0)
      asfPrintStatus("\nProcessing E2 granules ...\n");
    else if (strcmp_case(satellite, "J1") == 0)
      asfPrintStatus("\nProcessing J1 granules ...\n");
    else if (strcmp_case(satellite, "AS") == 0)
    	asfPrintStatus("\nProcessing AirSAR granules ...\n");
    else if (strcmp_case(satellite, "AI") == 0)
    	asfPrintStatus("\nProcessing AirMOSS granules ...\n");
    else if (strcmp_case(satellite, "UA") == 0)
    	asfPrintStatus("\nProcessing UAVSAR granules ...\n");

    // Determine how many granules we deal with
    granule_count = 0;
    asfPrintStatus("Determining granule count ...\n");
    fp = FOPEN(granule_table, "r");
    while (fgets(line, 1024, fp)) {
      if (is_granule(line, satellite, beam_mode, off_nadir))
				granule_count++;
    }
    FCLOSE(fp);
    asfPrintStatus("Found %ld %s %s granules in file (%s)\n", 
		   granule_count, satellite, beam_mode, granule_table);

    // Read granule information from file
    granule_t granule;
    granule_t *g = (granule_t *) MALLOC(sizeof(granule_t)*granule_count);
    nn = 0;
    asfPrintStatus("Reading granule table ...\n");
    fp = FOPEN(granule_table, "r");
    while (fgets(line, 1024, fp)) {
    	if (strstr(line, "GRANULE"))
    		continue;
      if (is_granule(line, satellite, beam_mode, off_nadir) &&
      		line2granule(line, satellite, beam_mode, off_nadir, &granule)) {
				g[nn] = granule;
				nn++;
      }
    }
    FCLOSE(fp);

		// No repeat-pass cycles for airborne data, so no stacks either.
		// Just write them out.
		if (strcmp_case(satellite, "AS") == 0 ||
				strcmp_case(satellite, "UA") == 0 ||
				strcmp_case(satellite, "AI") == 0) {
			for (kk=0; kk<nn; kk++)
	  	  fprintf(fpGran, "1,\"%s\",\"%s\",\"%s\",%.1lf,%d,%d,\"%s\","
		  	    		"\"%s\",\"%s\",%d,%d,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,"
		    	  		"%.4lf,%.4lf,%.4lf\n",
		      			g[nn].name, g[nn].satellite, g[nn].beam_mode, g[nn].off_nadir, 
		      			g[nn].orbit, g[nn].frame, g[nn].start_date, g[nn].end_date, 
		      			g[nn].orbit_dir, g[nn].path, g[nn].insar, g[nn].near_start_lat, 
			      		g[nn].near_start_lon, g[nn].far_start_lat, g[nn].far_start_lon,
			      		g[nn].near_end_lat, g[nn].near_end_lon, g[nn].far_end_lat, 
			      		g[nn].far_end_lon);
		}
		else {
			// Satellite data need some more work
    	frame_stack_t *stack = 
      	(frame_stack_t *) MALLOC(sizeof(frame_stack_t)*track_count);
	    for (kk=0; kk<track_count; kk++) // track numbers
  	    for (ll=0; ll<=max_frame_count; ll+=inc) // frames
					stack[kk].frame[ll] = 0;
    
    	// Count the frames in a stack
	    long foot_print_count = 0;
  	  for (nn=0; nn<granule_count; nn++) {
    	  orbit = g[nn].orbit;
      	track = orbit-(orbit/track_count)*track_count;
	      g[nn].track = track;
  	    if (stack[track].frame[g[nn].frame] == 0 &&
	  				strcmp_case(beam_mode, g[nn].beam_mode) == 0)
							foot_print_count++;
	      stack[track].frame[g[nn].frame]++;
  	  }
    	asfPrintStatus("Found %ld foot prints\n", foot_print_count);
    
	    for (kk=0; kk<track_count; kk++) {  
  	    printf("\rTrack: %03d/%d", kk+1, track_count);
    	  fflush(NULL);
      	for (ll=0; ll<=max_frame_count; ll+=inc) {
					frame_count = stack[kk].frame[ll];
					if (frame_count > 0) {
		  			granule_t *list = (granule_t *) MALLOC(sizeof(granule_t)*frame_count);
	  				for (nn=0; nn<granule_count; nn++) {
	    				mm = 0;
	    				if (g[nn].track == kk && g[nn].frame == ll) {
	    	  			list[mm].stack_id = stack_id + 1;
	      				list[mm].frame_count = frame_count;
	      				strcpy(list[mm].name, g[nn].name);
		      			strcpy(list[mm].satellite, g[nn].satellite);
			      		strcpy(list[mm].beam_mode, g[nn].beam_mode);
			      		list[mm].off_nadir = g[nn].off_nadir;
	  		    		list[mm].orbit = g[nn].orbit;
	    		  		list[mm].frame = g[nn].frame;
	      				strcpy(list[mm].start_date, g[nn].start_date);
	      				strcpy(list[mm].end_date, g[nn].end_date);
	      				strcpy(list[mm].orbit_dir, g[nn].orbit_dir);
			      		list[mm].path = g[nn].path;
			      		list[mm].near_start_lat = g[nn].near_start_lat;
	  		    		list[mm].near_start_lon = g[nn].near_start_lon;
	    		  		list[mm].far_start_lat = g[nn].far_start_lat;
	      				list[mm].far_start_lon = g[nn].far_start_lon;
	      				list[mm].near_end_lat = g[nn].near_end_lat;
	      				list[mm].near_end_lon = g[nn].near_end_lon;
		      			list[mm].far_end_lat = g[nn].far_end_lat;
			      		list[mm].far_end_lon = g[nn].far_end_lon;
		  	    		fprintf(fpGran, "%d,\"%s\",\"%s\",\"%s\",%.1lf,%d,%d,\"%s\","
			  	    		"\"%s\",\"%s\",%d,%d,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,"
		  	  	  		"%.4lf,%.4lf,%.4lf\n",
		    	  			stack_id+1, g[nn].name, g[nn].satellite, 
		      				g[nn].beam_mode, g[nn].off_nadir, g[nn].orbit, 
		      				g[nn].frame, g[nn].start_date, g[nn].end_date, g[nn].orbit_dir, 
			      			g[nn].path, g[nn].insar, g[nn].near_start_lat, 
			      			g[nn].near_start_lon, g[nn].far_start_lat, g[nn].far_start_lon,
				      		g[nn].near_end_lat, g[nn].near_end_lon, g[nn].far_end_lat, 
				      		g[nn].far_end_lon);
	  	    			mm++;
	    				}
	  				}
	  				if (mm > 0)
	    				my_sort(list, 0, mm-1);	  
	  
		 		 		FREE(list);
		  			stack_id++;
					}
      	}
    	}
    	FREE(g);
    }
  }
  FCLOSE(fpGran);
  printf("\n\n");

  // generate shape file
  asfPrintStatus("Generating shapefile ...\n");
  sprintf(outFile, "%s_%s_granules", satellite, beam_mode);
  granule2shape(granuleFile, outFile);

  return(0);
}
