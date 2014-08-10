#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <asf.h>
#include "asf_tiff.h"

#include <gsl/gsl_math.h>
#include <proj_api.h>

#include "asf_jpeg.h"
#include <png.h>
#include "envi.h"

#include "dateUtil.h"
#include <time.h>
#include "matrix.h"
#include <asf_nan.h>
#include <asf_endian.h>
#include <asf_meta.h>
#include <asf_export.h>
#include <asf_raster.h>
#include <float_image.h>
#include <spheroids.h>
#include <typlim.h>
#include <netcdf.h>
#include <xml_util.h>

#define RES 16
#define MAX_PTS 256

<<<<<<< HEAD
static xmlNode *findNode(xmlNode *node, char *name)
{
  xmlNode *cur = node->xmlChildrenNode;
  while (cur != NULL) {
    if (!xmlStrcmp(cur->name, (const xmlChar *)name))
      return cur;
    cur = cur->next;
  }
  return NULL;
}

static xmlNode *findXmlPtr(xmlDoc *doc, char *str)
{
  int ii, n;
  char **array;
  xmlNode *cur, *next;
  split_into_array(str, '.', &n, &array);
  cur = xmlDocGetRootElement(doc);
  for (ii=1; ii<n; ii++) {
    next = findNode(cur, array[ii]);
    cur = next;
  }
  free_char_array(&array, n);
  return cur;
}

static void xml_get_children(xmlNode *node, char **children)
{
  int ii=0;
  xmlNode *cur = node->xmlChildrenNode;
  for (cur = node; cur; cur = cur->next) {
    if (cur->type == XML_ELEMENT_NODE) {
      sprintf(children[ii], "%s", (char *)cur->name);
      ii++;
    }
  }
=======
int file_list_count(const char *listFile)
{
  char file[512];
  int file_count = 0;
  FILE *fp = FOPEN(listFile, "r");
  while (fgets(file, 512, fp)) {
    chomp(file);
    if (fileExists(file))
      file_count++;
    else
      asfPrintError("File (%s) does not exist\n", file);
  }  
  FCLOSE(fp);

  return file_count;
>>>>>>> rgps
}

void nc_meta_double(int group_id, char *name, char *desc, char *units,
		    double *value)
{
  int var_id;
  char *str = (char *) MALLOC(sizeof(char)*1024);
  nc_def_var(group_id, name, NC_DOUBLE, 0, 0, &var_id);
  nc_put_att_text(group_id, var_id, "long_name", strlen(desc), desc);
  if (units && strlen(units) > 0) {
    strcpy(str, units);
    nc_put_att_text(group_id, var_id, "units", strlen(str), str);
  }
  nc_put_var_double(group_id, var_id, value);
}

void nc_meta_float(int group_id, char *name, char *desc, char *units, 
		   float *value)
{
  int var_id;
  char *str = (char *) MALLOC(sizeof(char)*1024);
  nc_def_var(group_id, name, NC_FLOAT, 0, 0, &var_id);
  nc_put_att_text(group_id, var_id, "long_name", strlen(desc), desc);
  if (units && strlen(units) > 0) {
    strcpy(str, units);
    nc_put_att_text(group_id, var_id, "units", strlen(str), str);
  }
  nc_put_var_float(group_id, var_id, value);
}

void nc_meta_int(int group_id, char *name, char *desc, char *units,
		 int *value)
{
  int var_id;
  char *str = (char *) MALLOC(sizeof(char)*1024);
  nc_def_var(group_id, name, NC_INT, 0, 0, &var_id);
  nc_put_att_text(group_id, var_id, "long_name", strlen(desc), desc);
  if (units && strlen(units) > 0) {
    strcpy(str, units);
    nc_put_att_text(group_id, var_id, "units", strlen(str), str);
  }
  nc_put_var_int(group_id, var_id, value);
}

void nc_meta_str(int group_id, char *name, char *desc, char *units,
		 char *value)
{
  int var_id;
  char *str_value = (char *) MALLOC(sizeof(char)*(1+strlen(value)));
  strcpy(str_value, value);
  char *str = (char *) MALLOC(sizeof(char)*1024);
  nc_def_var(group_id, name, NC_STRING, 0, 0, &var_id);
  nc_put_att_text(group_id, var_id, "long_name", strlen(desc), desc);
  if (units && strlen(units) > 0) {
    strcpy(str, units);
    nc_put_att_text(group_id, var_id, "units", strlen(str), str);
  }
  nc_put_var_string(group_id, var_id, (const char **)&str_value);
}

<<<<<<< HEAD
static void add_var_attr(xmlDoc *doc, int ncid, int var_id, char *xml_id)
{
  int ii;
  int meta_count = xml_get_children_count(doc, xml_id);
  char **meta_param = (char **) MALLOC(sizeof(char *)*meta_count);
  for (ii=0; ii<meta_count; ii++)
    meta_param[ii] = (char *) MALLOC(sizeof(char)*50);
  xmlNodePtr node = findXmlPtr(doc, xml_id);
  xml_get_children(node->children, meta_param);
  char type[10], xmlStr[512], str[512];
  int nValue;
  double fValue;
  for (ii=0; ii<meta_count; ii++) {
    sprintf(xmlStr, "%s.%s.type", xml_id, meta_param[ii]);
    strcpy(type, xml_get_string_attribute(doc, xmlStr));
    sprintf(xmlStr, "%s.%s", xml_id, meta_param[ii]);
    if (strcmp_case(type, "STRING") == 0) {
      strcpy(str, xml_get_string_value(doc, xmlStr));
      nc_put_att_text(ncid, var_id, meta_param[ii], strlen(str), str);
    }
    else if (strcmp_case(type, "INTEGER") == 0) {
      nValue = xml_get_int_value(doc, xmlStr);
      nc_put_att_int(ncid, var_id, meta_param[ii], NC_INT, 1, &nValue);
    }
    else if (strcmp_case(type, "DOUBLE") == 0) {
      fValue = xml_get_double_value(doc, xmlStr);
      nc_put_att_double(ncid, var_id, meta_param[ii], NC_DOUBLE, 1, &fValue);
    }
  }
}
=======
netcdf_t *initialize_netcdf_file(const char *output_file, band_t *band,
				 int band_count, meta_parameters *meta, 
				 int meta_structure, int metadata_xml)
{
  int ii, status, ncid, var_id, file_count=0, dim_count=0;
  char *spatial_ref=NULL, *datum=NULL, *spheroid=NULL;
  int dim_time_id, dim_height_id, dim_width_id, dim_depth_id, dim_bounds_id;

  // Work out file count and dim count
  for (ii=0; ii<band_count; ii++) {
    if (band[ii].time_count > file_count)
      file_count = band[ii].time_count;
    if (band[ii].dim_count > dim_count)
      dim_count = band[ii].dim_count;
  }
>>>>>>> rgps

void export_netcdf(const char *in_base_name, char *output_file_name,
  int *noutputs, char ***output_names)
{
  int hh, ii, jj, kk;
  char image_file_name[1024], data_file_name[1024], xmlStr[512];
  
  // Check out the general setup
  xmlDoc *doc = xmlReadFile(in_base_name, NULL, 0);
  int variable_count = xml_get_children_count(doc, "hdf5.data");
  if (variable_count == 0)
    asfPrintError("Could not find any data entries!\n");
    
  // Extract names of data sets and get metadata
  char **data_set = (char **) MALLOC(sizeof(char *)*variable_count);
  for (ii=0; ii<variable_count; ii++)
    data_set[ii] = (char *) MALLOC(sizeof(char)*50);
  xmlNode *node = findXmlPtr(doc, "hdf5.data");
  xml_get_children(node->children, data_set);
  
  int found_lat = FALSE, found_lon = FALSE;
  for (ii=0; ii<variable_count; ii++) {
    if (strcmp_case(data_set[ii], "LATITUDE") == 0)
      found_lat = TRUE;
    if (strcmp_case(data_set[ii], "LONGITUDE") == 0)
      found_lon = TRUE;
  }
  if (!found_lat)
    asfPrintError("Could not find latitude information!\n");
  if (!found_lon)
    asfPrintError("Could not find longitude information!\n");

  sprintf(xmlStr, "hdf5.data.%s", data_set[0]);
  strcpy(image_file_name, xml_get_string_value(doc, xmlStr));
  char *p = strstr(image_file_name, ":");
  if (p)
    *p = '\0';
  strcpy(data_file_name, image_file_name);
  meta_parameters *meta = meta_read(image_file_name);

  // Assign parameters
<<<<<<< HEAD
  int var_count = variable_count + 1;
  int projected = FALSE;
  if (meta->projection && meta->projection->type != SCANSAR_PROJECTION) {
    projected = TRUE;
    var_count += 3;
  }
  size_t line_count = meta->general->line_count;
  size_t sample_count = meta->general->sample_count;
  int band_count = meta->general->band_count;
  
  // Assign data type
  nc_type datatype;
  if (meta->general->data_type == ASF_BYTE)
    datatype = NC_CHAR;
  else if (meta->general->data_type == REAL32)
    datatype = NC_FLOAT;

  // Initialize the netCDF pointer structure
  netcdf_t *netcdf = (netcdf_t *) MALLOC(sizeof(netcdf_t));
  netcdf->var_count = var_count;
  netcdf->var_id = (int *) MALLOC(sizeof(int)*var_count);
=======
  size_t line_count = mg->line_count;
  size_t sample_count = mg->sample_count;

  // Initialize the netCDF pointer structure
  netcdf_t *netcdf = (netcdf_t *) MALLOC(sizeof(netcdf_t));
  netcdf->var_count = band_count;
  netcdf->var_id = (int *) MALLOC(sizeof(int)*band_count);
>>>>>>> rgps

  // Create output file
  int ncid;
  append_ext_if_needed(output_file_name, ".nc", NULL);
  int status = nc_create(output_file_name, NC_CLOBBER|NC_NETCDF4, &ncid);
  netcdf->ncid = ncid;
  if (status != NC_NOERR)
<<<<<<< HEAD
    asfPrintError("Could not open netCDF file (%s)!\n", nc_strerror(status));
    
  // Define dimensions
  int dim_xgrid_id, dim_ygrid_id, dim_lat_id, dim_lon_id, dim_time_id;
  if (projected) {
    status = nc_def_dim(ncid, "xgrid", sample_count, &dim_xgrid_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with xgrid variable definition!\n");
    status = nc_def_dim(ncid, "ygrid", line_count, &dim_ygrid_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with ygrid variable definition!\n");
  }
  status = nc_def_dim(ncid, "longitude", sample_count, &dim_lon_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with longitude variable definition!\n");
  status = nc_def_dim(ncid, "latitude", line_count, &dim_lat_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with latitude variable definition!\n");
  // FIXME - currently only covers single time
  status = nc_def_dim(ncid, "time", 1, &dim_time_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with time variable definition!\n");
    
  int dims_bands[3];
  dims_bands[2] = dim_time_id;
  if (projected) {
    dims_bands[0] = dim_ygrid_id;
    dims_bands[1] = dim_xgrid_id;
  }
  else {
    dims_bands[0] = dim_lon_id;
    dims_bands[1] = dim_lat_id;
  }

  // Define projection
  double fValue;
  int nn = 0, var_id;
  char str[512];
  int meta_count = xml_get_children_count(doc, "hdf5.projection");
  if (projected && meta_count > 0) {
    nc_def_var(ncid, "projection", NC_INT, 0, 0, &var_id);
    netcdf->var_id[nn] = var_id;
    char **proj_param = (char **) MALLOC(sizeof(char *)*meta_count);
    for (ii=0; ii<meta_count; ii++)
      proj_param[ii] = (char *) MALLOC(sizeof(char)*50);
    node = findXmlPtr(doc, "hdf5.projection");
    xml_get_children(node->children, proj_param);
    for (ii=0; ii<meta_count; ii++) {
      sprintf(xmlStr, "hdf5.projection.%s", proj_param[ii]);
      if (strcmp_case(proj_param[ii], "false_easting") == 0 ||
        strcmp_case(proj_param[ii], "false_northing") == 0 ||
        strcmp_case(proj_param[ii], "latitude_of_projection_origin") == 0 ||
        strcmp_case(proj_param[ii], "longitude_of_central_meridian") == 0 ||
        strcmp_case(proj_param[ii], "longitude_of_projection_origin") == 0 ||
        strcmp_case(proj_param[ii], "scale_factor_at_central_meridian") == 0 ||
        strcmp_case(proj_param[ii], "semi_major_axis") == 0 ||
        strcmp_case(proj_param[ii], "semi_minor_axis") == 0 ||
        strcmp_case(proj_param[ii], "standard_parallel") == 0 ||
        strcmp_case(proj_param[ii], "standard_parallel_1") == 0 ||
        strcmp_case(proj_param[ii], "standard_parallel_2") == 0 ||
        strcmp_case(proj_param[ii], 
          "straight_vertical_longitude_from_pole") == 0) {
        fValue = xml_get_double_value(doc, xmlStr);
        nc_put_att_double(ncid, var_id, proj_param[ii], NC_DOUBLE, 1, &fValue);
      }
      else if (strcmp_case(proj_param[ii], "grid_mapping_name") == 0) {
        strcpy(str, xml_get_string_value(doc, xmlStr));
        nc_put_att_text(ncid, var_id, proj_param[ii], strlen(str), str);
      }
      else
        asfPrintError("Undefined projection parameter (%s)!\n", proj_param[ii]);
      FREE(proj_param[ii]);
    }
    FREE(proj_param);
  }

  if (projected) {

    // Define ygrid
    nn++;
    int dims_ygrid[2] = { dim_ygrid_id, dim_xgrid_id };
    nc_def_var(ncid, "ygrid", NC_FLOAT, 2, dims_ygrid, &var_id);
    netcdf->var_id[nn] = var_id;
    nc_def_var_deflate(ncid, var_id, 0, 1, 6);
    add_var_attr(doc, ncid, var_id, "hdf5.metadata.ygrid");
    
    // Define xgrid
    nn++;
    int dims_xgrid[2] = { dim_ygrid_id, dim_xgrid_id };
    nc_def_var(ncid, "xgrid", NC_FLOAT, 2, dims_xgrid, &var_id);
    netcdf->var_id[nn] = var_id;
    nc_def_var_deflate(ncid, var_id, 0, 1, 6);
    add_var_attr(doc, ncid, var_id, "hdf5.metadata.xgrid");
  }

  // Define longitude
  nn++;
  if (projected) {
    int dims_lon[2] = { dim_ygrid_id, dim_xgrid_id };
    nc_def_var(ncid, "longitude", NC_FLOAT, 2, dims_lon, &var_id);
  }
  else {
    int dims_lon[2] = { dim_lon_id, dim_lat_id };
    nc_def_var(ncid, "longitude", NC_FLOAT, 2, dims_lon, &var_id);
  }
  netcdf->var_id[nn] = var_id;
  nc_def_var_deflate(ncid, var_id, 0, 1, 6);
  add_var_attr(doc, ncid, var_id, "hdf5.metadata.lon");
  
  // Define latitude
  nn++;
  if (projected) {
    int dims_lat[2] = { dim_ygrid_id, dim_xgrid_id };
    nc_def_var(ncid, "latitude", NC_FLOAT, 2, dims_lat, &var_id);
  }
  else {
    int dims_lat[2] = { dim_lat_id, dim_lon_id };
    nc_def_var(ncid, "latitude", NC_FLOAT, 2, dims_lat, &var_id);
  }
  netcdf->var_id[nn] = var_id;
  nc_def_var_deflate(ncid, var_id, 0, 1, 6);
  add_var_attr(doc, ncid, var_id, "hdf5.metadata.lat");

  // Define time
  nn++;
  int dims_time[1] = { dim_time_id };
  nc_def_var(ncid, "time", NC_FLOAT, 1, dims_time, &var_id);
  netcdf->var_id[nn] = var_id;
  add_var_attr(doc, ncid, var_id, "hdf5.metadata.time");

  // Define image variables
  char band[25];
  for (ii=0; ii<variable_count; ii++) {
  
    // Skip lat/lon layers
    if (strcmp_case(data_set[ii], "LATITUDE") == 0 ||
      strcmp_case(data_set[ii], "LONGITUDE") == 0)
      continue;
      
    // Take care of all other layers
    nn++;
    nc_def_var(ncid, data_set[ii], datatype, 3, dims_bands, &var_id);
    netcdf->var_id[nn] = var_id;
    nc_def_var_deflate(ncid, var_id, 0, 1, 6);
    sprintf(xmlStr, "hdf5.data.%s", data_set[ii]);
    strcpy(image_file_name, xml_get_string_value(doc, xmlStr));

    // check whether metadata is stored by band name or not
    char *p = strstr(image_file_name, ":");
    if (p)
      sprintf(band, "hdf5.metadata.%s", p+1);
    else
      sprintf(band, "hdf5.metadata.%s", data_set[ii]);
    add_var_attr(doc, ncid, var_id, band);
=======
    asfPrintError("Could not open netCDF file (%s).\n", nc_strerror(status));
  
  // Define projection
  int dim_str[1] = 50;
  char str[512];
  double fValue;
  int projected = FALSE;
  if (mp)
    projected = TRUE;
  if (projected) {
    status = nc_def_var(ncid, "projection", NC_CHAR, 1, dim_str, &var_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with 'projection' variable definition\n");
    if (mp->type == UNIVERSAL_TRANSVERSE_MERCATOR) {

      strcpy(str, "transverse_mercator");
      nc_put_att_text(ncid, var_id, "grid_mapping_name", strlen(str), str);
      nc_put_att_double(ncid, var_id, "scale_factor_at_central_meridian", 
			NC_DOUBLE, 1, &mp->param.utm.scale_factor);
      nc_put_att_double(ncid, var_id, "longitude_of_central_meridian",
			NC_DOUBLE, 1, &mp->param.utm.lon0);
      nc_put_att_double(ncid, var_id, "latitude_of_projection_origin",
			NC_DOUBLE, 1, &mp->param.utm.lat0);
      nc_put_att_double(ncid, var_id, "false_easting", NC_DOUBLE, 1, 
			&mp->param.utm.false_easting);
      nc_put_att_double(ncid, var_id, "false_northing", NC_DOUBLE, 1, 
			&mp->param.utm.false_northing);
      strcpy(str, "xgrid");
      nc_put_att_text(ncid, var_id, "projection_x_coordinate", strlen(str), 
		      str);
      strcpy(str, "ygrid");
      nc_put_att_text(ncid, var_id, "projection_y_coordinate", strlen(str), 
		      str);
      strcpy(str, "m");
      nc_put_att_text(ncid, var_id, "units", strlen(str), str); 
      nc_put_att_double(ncid, var_id, "grid_boundary_top_projected_y",
			NC_DOUBLE, 1, &mp->startY);
      fValue = mp->startY + mg->line_count * mp->perY;
      nc_put_att_double(ncid, var_id, "grid_boundary_bottom_projected_y",
			NC_DOUBLE, 1, &fValue);
      fValue = mp->startX + mg->sample_count * mp->perX;
      nc_put_att_double(ncid, var_id, "grid_boundary_right_projected_x",
			NC_DOUBLE, 1, &fValue);
      nc_put_att_double(ncid, var_id, "grid_boundary_left_projected_x",
			NC_DOUBLE, 1, &mp->startX);
      spatial_ref = (char *) MALLOC(sizeof(char)*1024);
      datum = (char *) datum_toString(mp->datum);
      spheroid = (char *) spheroid_toString(mp->spheroid);
      double flat = mp->re_major/(mp->re_major - mp->re_minor);
      sprintf(spatial_ref, "PROJCS[\"%s_UTM_Zone_%d%c\",GEOGCS[\"GCS_%s\",DATUM[\"D_%s\",SPHEROID[\"%s\",%.1f,%-16.11g]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"False_Easting\",%.1f],PARAMETER[\"False_Northing\",%.1f],PARAMETER[\"Central_Meridian\",%.1f],PARAMETER[\"Scale_Factor\",%.4f],PARAMETER[\"Latitude_Of_Origin\",%.1f],UNIT[\"Meter\",1]]",
	      spheroid, mp->param.utm.zone, mp->hem, spheroid, datum, 
	      spheroid, mp->re_major, flat, mp->param.utm.false_easting, 
	      mp->param.utm.false_northing, mp->param.utm.lon0, 
	      mp->param.utm.scale_factor, mp->param.utm.lat0);
      nc_put_att_text(ncid, var_id, "spatial_ref", strlen(spatial_ref), 
		      spatial_ref);
      sprintf(str, "+proj=utm +zone=%d", mp->param.utm.zone);
      if (meta->general->center_latitude < 0)
	      strcat(str, " +south");
      nc_put_att_text(ncid, var_id, "proj4text", strlen(str), str);
      nc_put_att_int(ncid, var_id, "zone", NC_INT, 1, &mp->param.utm.zone);
      nc_put_att_double(ncid, var_id, "semimajor_radius", NC_DOUBLE, 1, 
			&mp->re_major);
      nc_put_att_double(ncid, var_id, "semiminor_radius", NC_DOUBLE, 1, 
			&mp->re_minor);
      sprintf(str, "%.6f %.6f 0 %.6f 0 %.6f", mp->startX, mp->perX, 
	      mp->startY, mp->perY); 
      nc_put_att_text(ncid, var_id, "GeoTransform", strlen(str), str);
    }
    else if (mp->type == POLAR_STEREOGRAPHIC) {

      strcpy(str, "polar_stereographic");
      nc_put_att_text(ncid, var_id, "grid_mapping_name", strlen(str), str);
      fValue = 90.0;
      nc_put_att_double(ncid, var_id, "straight_vertical_longitude_from_pole", 
			NC_DOUBLE, 1, &mp->param.ps.slon);
      nc_put_att_double(ncid, var_id, "longitude_of_central_meridian",
			NC_DOUBLE, 1, &fValue);
      nc_put_att_double(ncid, var_id, "standard_parallel",
			NC_DOUBLE, 1, &mp->param.ps.slat);
      nc_put_att_double(ncid, var_id, "false_easting", NC_DOUBLE, 1, 
			&mp->param.ps.false_easting);
      nc_put_att_double(ncid, var_id, "false_northing", NC_DOUBLE, 1, 
			&mp->param.ps.false_northing);
      strcpy(str, "xgrid");
      nc_put_att_text(ncid, var_id, "projection_x_coordinate", strlen(str), 
		      str);
      strcpy(str, "ygrid");
      nc_put_att_text(ncid, var_id, "projection_y_coordinate", strlen(str), 
		      str);
      strcpy(str, "m");
      nc_put_att_text(ncid, var_id, "units", strlen(str), str); 
      nc_put_att_double(ncid, var_id, "grid_boundary_top_projected_y",
			NC_DOUBLE, 1, &mp->startY);
      fValue = mp->startY + mg->line_count * mp->perY;
      nc_put_att_double(ncid, var_id, "grid_boundary_bottom_projected_y",
			NC_DOUBLE, 1, &fValue);
      fValue = mp->startX + mg->sample_count * mp->perX;
      nc_put_att_double(ncid, var_id, "grid_boundary_right_projected_x",
			NC_DOUBLE, 1, &fValue);
      nc_put_att_double(ncid, var_id, "grid_boundary_left_projected_x",
			NC_DOUBLE, 1, &mp->startX);
      spatial_ref = (char *) MALLOC(sizeof(char)*1024);
      datum = (char *) datum_toString(mp->datum);
      spheroid = (char *) spheroid_toString(mp->spheroid);
      double flat = mp->re_major/(mp->re_major - mp->re_minor);
      sprintf(spatial_ref, "PROJCS[\"Stereographic_North_Pole\",GEOGCS[\"unnamed ellipse\",DATUM[\"D_unknown\",SPHEROID[\"Unknown\",%.3f,%-16.11g]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.0002247191011236]],PROJECTION[\"Stereographic_North_Pole\"],PARAMETER[\"standard_parallel_1\",%.4f],PARAMETER[\"central_meridian\",%.4f],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",%.3f],PARAMETER[\"false_northing\",%.3f],UNIT[\"Meter\",1,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"3411\"]]",
	      mp->re_major, flat, mp->param.ps.slat, mp->param.ps.slon,
	      mp->param.ps.false_easting, mp->param.ps.false_northing);
      nc_put_att_text(ncid, var_id, "spatial_ref", strlen(spatial_ref), 
		      spatial_ref);
      if (mp->param.ps.is_north_pole)
	sprintf(str, "+proj=stere +lat_0=90.0000 +lat_ts=%.4f "
		"+lon_0=%.4f +k=1 +x_0=%.3f +y_0=%.3f +a=%.3f +b=%.3f "
		"+units=m +no_defs", mp->param.ps.slat, mp->param.ps.slon,
		mp->param.ps.false_easting, mp->param.ps.false_northing,
		mp->re_major, mp->re_minor);
      else
	sprintf(str, "+proj=stere +lat_0=-90.0000 +lat_ts=%.4f "
		"+lon_0=%.4f +k=1 +x_0=%.3f +y_0=%.3f +a=%.3f +b=%.3f "
		"+units=m +no_defs", mp->param.ps.slat, mp->param.ps.slon,
		mp->param.ps.false_easting, mp->param.ps.false_northing,
		mp->re_major, mp->re_minor);
      nc_put_att_text(ncid, var_id, "proj4text", strlen(str), str);
      nc_put_att_double(ncid, var_id, "latitude_of_true_scale", NC_DOUBLE, 1,
			&mp->param.ps.slat);
      nc_put_att_double(ncid, var_id, "longitude_of_projection_origin", 
			NC_DOUBLE, 1, &mp->param.ps.slon);
      nc_put_att_double(ncid, var_id, "semimajor_radius", NC_DOUBLE, 1, 
			&mp->re_major);
      nc_put_att_double(ncid, var_id, "semiminor_radius", NC_DOUBLE, 1, 
			&mp->re_minor);
      sprintf(str, "%.6f %.6f 0 %.6f 0 %.6f", mp->startX, mp->perX, 
	      mp->startY, mp->perY); 
      nc_put_att_text(ncid, var_id, "GeoTransform", strlen(str), str);
    }
    else if (mp->type == ALBERS_EQUAL_AREA) {

      strcpy(str, "albers_conical_equal_area");
      nc_put_att_text(ncid, var_id, "grid_mapping_name", strlen(str), str);
      fValue = 90.0;
      nc_put_att_double(ncid, var_id, "standard_parallel_1", 
			NC_DOUBLE, 1, &mp->param.albers.std_parallel1);
      nc_put_att_double(ncid, var_id, "standard_parallel_2",
			NC_DOUBLE, 1, &mp->param.albers.std_parallel2);
      nc_put_att_double(ncid, var_id, "longitude_of_central_meridian",
			NC_DOUBLE, 1, &mp->param.albers.center_meridian);
      nc_put_att_double(ncid, var_id, "latitude_of_projection_origin",
			NC_DOUBLE, 1, &mp->param.albers.orig_latitude);
      nc_put_att_double(ncid, var_id, "false_easting", NC_DOUBLE, 1, 
			&mp->param.albers.false_easting);
      nc_put_att_double(ncid, var_id, "false_northing", NC_DOUBLE, 1, 
			&mp->param.albers.false_northing);
      strcpy(str, "xgrid");
      nc_put_att_text(ncid, var_id, "projection_x_coordinate", strlen(str), 
		      str);
      strcpy(str, "ygrid");
      nc_put_att_text(ncid, var_id, "projection_y_coordinate", strlen(str), 
		      str);
      strcpy(str, "m");
      nc_put_att_text(ncid, var_id, "units", strlen(str), str); 
      nc_put_att_double(ncid, var_id, "grid_boundary_top_projected_y",
			NC_DOUBLE, 1, &mp->startY);
      fValue = mp->startY + mg->line_count * mp->perY;
      nc_put_att_double(ncid, var_id, "grid_boundary_bottom_projected_y",
			NC_DOUBLE, 1, &fValue);
      fValue = mp->startX + mg->sample_count * mp->perX;
      nc_put_att_double(ncid, var_id, "grid_boundary_right_projected_x",
			NC_DOUBLE, 1, &fValue);
      nc_put_att_double(ncid, var_id, "grid_boundary_left_projected_x",
			NC_DOUBLE, 1, &mp->startX);
      spatial_ref = (char *) MALLOC(sizeof(char)*1024);
      datum = (char *) datum_toString(mp->datum);
      spheroid = (char *) spheroid_toString(mp->spheroid);
      double flat = mp->re_major/(mp->re_major - mp->re_minor);
      sprintf(spatial_ref, "PROJCS[\"Albers_Equal_Area_Conic\",GEOGCS[\"GCS_%s\",DATUM[\"D_%s\",SPHEROID[\"%s\",%.3f,%-16.11g]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.0174532925199432955]],PROJECTION[\"Albers\"],PARAMETER[\"False_Easting\",%.3f],PARAMETER[\"False_Northing\",%.3f],PARAMETER[\"Central_Meridian\",%.4f],PARAMETER[\"Standard_Parallel_1\",%.4f],PARAMETER[\"Standard_Parallel_2\",%.4f],PARAMETER[\"Latitude_Of_Origin\",%.4f],UNIT[\"Meter\",1]]",
	      datum, datum, spheroid, mp->re_major, flat, 
	      mp->param.albers.false_easting, mp->param.albers.false_northing,
	      mp->param.albers.center_meridian, mp->param.albers.std_parallel1,
	      mp->param.albers.std_parallel2, mp->param.albers.orig_latitude);
      nc_put_att_text(ncid, var_id, "spatial_ref", strlen(spatial_ref), 
		      spatial_ref);
      sprintf(str, "+proj=aea +lat_1=%.4f +lat_2=%.4f +lat_0=%.4f "
	      "+lon_0=%.4f +x_0=%.3f +y_0=%.3f", 
	      mp->param.albers.std_parallel1, mp->param.albers.std_parallel2, 
	      mp->param.albers.orig_latitude, mp->param.albers.center_meridian,
	      mp->param.albers.false_easting, mp->param.albers.false_northing);
      nc_put_att_text(ncid, var_id, "proj4text", strlen(str), str);
      nc_put_att_double(ncid, var_id, "latitude_of_true_scale", NC_DOUBLE, 1,
			&mp->param.ps.slat);
      nc_put_att_double(ncid, var_id, "longitude_of_projection_origin", 
			NC_DOUBLE, 1, &mp->param.ps.slon);
      nc_put_att_double(ncid, var_id, "semimajor_radius", NC_DOUBLE, 1, 
			&mp->re_major);
      nc_put_att_double(ncid, var_id, "semiminor_radius", NC_DOUBLE, 1, 
			&mp->re_minor);
      sprintf(str, "%.6f %.6f 0 %.6f 0 %.6f", mp->startX, mp->perX, 
	      mp->startY, mp->perY); 
      nc_put_att_text(ncid, var_id, "GeoTransform", strlen(str), str);
    }
    else if (mp->type == LAMBERT_CONFORMAL_CONIC) {

      strcpy(str, "lambert_conformal_conic");
      nc_put_att_text(ncid, var_id, "grid_mapping_name", strlen(str), str);
      fValue = 90.0;
      nc_put_att_double(ncid, var_id, "standard_parallel_1", 
			NC_DOUBLE, 1, &mp->param.lamcc.plat1);
      nc_put_att_double(ncid, var_id, "standard_parallel_2",
			NC_DOUBLE, 1, &mp->param.lamcc.plat2);
      nc_put_att_double(ncid, var_id, "longitude_of_central_meridian",
			NC_DOUBLE, 1, &mp->param.lamcc.lon0);
      nc_put_att_double(ncid, var_id, "latitude_of_projection_origin",
			NC_DOUBLE, 1, &mp->param.lamcc.lat0);
      nc_put_att_double(ncid, var_id, "false_easting", NC_DOUBLE, 1, 
			&mp->param.lamcc.false_easting);
      nc_put_att_double(ncid, var_id, "false_northing", NC_DOUBLE, 1, 
			&mp->param.lamcc.false_northing);
      strcpy(str, "xgrid");
      nc_put_att_text(ncid, var_id, "projection_x_coordinate", strlen(str), 
		      str);
      strcpy(str, "ygrid");
      nc_put_att_text(ncid, var_id, "projection_y_coordinate", strlen(str), 
		      str);
      strcpy(str, "m");
      nc_put_att_text(ncid, var_id, "units", strlen(str), str); 
      nc_put_att_double(ncid, var_id, "grid_boundary_top_projected_y",
			NC_DOUBLE, 1, &mp->startY);
      fValue = mp->startY + mg->line_count * mp->perY;
      nc_put_att_double(ncid, var_id, "grid_boundary_bottom_projected_y",
			NC_DOUBLE, 1, &fValue);
      fValue = mp->startX + mg->sample_count * mp->perX;
      nc_put_att_double(ncid, var_id, "grid_boundary_right_projected_x",
			NC_DOUBLE, 1, &fValue);
      nc_put_att_double(ncid, var_id, "grid_boundary_left_projected_x",
			NC_DOUBLE, 1, &mp->startX);
      spatial_ref = (char *) MALLOC(sizeof(char)*1024);
      datum = (char *) datum_toString(mp->datum);
      spheroid = (char *) spheroid_toString(mp->spheroid);
      double flat = mp->re_major/(mp->re_major - mp->re_minor);
      sprintf(spatial_ref, "PROJCS[\"Lambert_Conformal_Conic\",GEOGCS[\"GCS_%s\",DATUM[\"D_%s\",SPHEROID[\"%s\",%.3f,%-16.11g]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.0174532925199432955]],PROJECTION[\"Lambert_Conformal_Conic\"],PARAMETER[\"False_Easting\",%.3f],PARAMETER[\"False_Northing\",%.3f],PARAMETER[\"Central_Meridian\",%.4f],PARAMETER[\"Standard_Parallel_1\",%.4f],PARAMETER[\"Standard_Parallel_2\",%.4f],PARAMETER[\"Latitude_Of_Origin\",%.4f],UNIT[\"Meter\",1]]",
	      datum, datum, spheroid, mp->re_major, flat, 
	      mp->param.lamcc.false_easting, mp->param.lamcc.false_northing,
	      mp->param.lamcc.lon0, mp->param.lamcc.plat1,
	      mp->param.lamcc.plat2, mp->param.lamcc.lat0);
      nc_put_att_text(ncid, var_id, "spatial_ref", strlen(spatial_ref), 
		      spatial_ref);
      sprintf(str, "+proj=lcc +lat_1=%.4f +lat_2=%.4f +lat_0=%.4f "
	      "+lon_0=%.4f +x_0=%.3f +y_0=%.3f", 
	      mp->param.lamcc.plat1, mp->param.lamcc.plat2,
	      mp->param.lamcc.lat0, mp->param.lamcc.lon0,
	      mp->param.lamcc.false_easting, mp->param.lamcc.false_northing);
      nc_put_att_text(ncid, var_id, "proj4text", strlen(str), str);
      nc_put_att_double(ncid, var_id, "semimajor_radius", NC_DOUBLE, 1, 
			&mp->re_major);
      nc_put_att_double(ncid, var_id, "semiminor_radius", NC_DOUBLE, 1, 
			&mp->re_minor);
      sprintf(str, "%.6f %.6f 0 %.6f 0 %.6f", mp->startX, mp->perX, 
	      mp->startY, mp->perY); 
      nc_put_att_text(ncid, var_id, "GeoTransform", strlen(str), str);
    }
    else if (mp->type == LAMBERT_AZIMUTHAL_EQUAL_AREA) {

      strcpy(str, "lambert_azimuthal_equal_area");
      nc_put_att_text(ncid, var_id, "grid_mapping_name", strlen(str), str);
      nc_put_att_double(ncid, var_id, "longitude_of_projection_origin",
			NC_DOUBLE, 1, &mp->param.lamaz.center_lon);
      nc_put_att_double(ncid, var_id, "latitude_of_projection_origin",
			NC_DOUBLE, 1, &mp->param.lamaz.center_lat);
      nc_put_att_double(ncid, var_id, "false_easting", NC_DOUBLE, 1, 
			&mp->param.lamaz.false_easting);
      nc_put_att_double(ncid, var_id, "false_northing", NC_DOUBLE, 1, 
			&mp->param.lamaz.false_northing);
      strcpy(str, "xgrid");
      nc_put_att_text(ncid, var_id, "projection_x_coordinate", strlen(str), 
		      str);
      strcpy(str, "ygrid");
      nc_put_att_text(ncid, var_id, "projection_y_coordinate", strlen(str), 
		      str);
      strcpy(str, "m");
      nc_put_att_text(ncid, var_id, "units", strlen(str), str); 
      nc_put_att_double(ncid, var_id, "grid_boundary_top_projected_y",
			NC_DOUBLE, 1, &mp->startY);
      fValue = mp->startY + mg->line_count * mp->perY;
      nc_put_att_double(ncid, var_id, "grid_boundary_bottom_projected_y",
			NC_DOUBLE, 1, &fValue);
      fValue = mp->startX + mg->sample_count * mp->perX;
      nc_put_att_double(ncid, var_id, "grid_boundary_right_projected_x",
			NC_DOUBLE, 1, &fValue);
      nc_put_att_double(ncid, var_id, "grid_boundary_left_projected_x",
			NC_DOUBLE, 1, &mp->startX);
      spatial_ref = (char *) MALLOC(sizeof(char)*1024);
      datum = (char *) datum_toString(mp->datum);
      spheroid = (char *) spheroid_toString(mp->spheroid);
      double flat = mp->re_major/(mp->re_major - mp->re_minor);
      sprintf(spatial_ref, "PROJCS[\"Lambert_Azimuthal_Equal_Area\",GEOGCS[\"GCS_%s\",DATUM[\"D_%s\",SPHEROID[\"%s\",%.3f,%-16.11g]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.0174532925199432955]],PROJECTION[\"Lambert_Conformal_Conic\"],PARAMETER[\"False_Easting\",%.3f],PARAMETER[\"False_Northing\",%.3f],PARAMETER[\"Central_Meridian\",%.4f],PARAMETER[\"Latitude_Of_Origin\",%.4f],UNIT[\"Meter\",1]]",
	      datum, datum, spheroid, mp->re_major, flat, 
	      mp->param.lamaz.false_easting, mp->param.lamaz.false_northing,
	      mp->param.lamaz.center_lon, mp->param.lamaz.center_lat);
      nc_put_att_text(ncid, var_id, "spatial_ref", strlen(spatial_ref), 
		      spatial_ref);
      sprintf(str, "+proj=laea +lat_0=%.4f +lon_0=%.4f +x_0=%.3f "
	      "+y_0=%.3f", 
	      mp->param.lamaz.center_lat, mp->param.lamaz.center_lon,
	      mp->param.lamaz.false_easting, mp->param.lamaz.false_northing);
      nc_put_att_text(ncid, var_id, "proj4text", strlen(str), str);
      nc_put_att_double(ncid, var_id, "semimajor_radius", NC_DOUBLE, 1, 
			&mp->re_major);
      nc_put_att_double(ncid, var_id, "semiminor_radius", NC_DOUBLE, 1, 
			&mp->re_minor);
      sprintf(str, "%.6f %.6f 0 %.6f 0 %.6f", mp->startX, mp->perX, 
	      mp->startY, mp->perY); 
      nc_put_att_text(ncid, var_id, "GeoTransform", strlen(str), str);
    }
  }

  // Define dimensions
  status = nc_def_dim(ncid, "lon", sample_count, &dim_width_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with 'longitude' dimension definition\n");
  status = nc_def_dim(ncid, "lat", line_count, &dim_height_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with 'latitude' dimension definition\n");
  status = nc_def_dim(ncid, "time", file_count, &dim_time_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with 'time' dimension definition\n");
  status = nc_def_dim(ncid, "nv", 2, &dim_bounds_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with 'nv' dimension definition\n");
  if (dim_count == 4) {
    if (strcmp_case(band[0].name, "backscatter_histogram") == 0)
      status = nc_def_dim(ncid, "histogram_bin", band[0].cat_count, 
			  &dim_depth_id);
    else if (strcmp_case(band[0].name, "ice_age") == 0)
      status = nc_def_dim(ncid, "age_category", band[0].cat_count, 
			  &dim_depth_id);
    else if (strcmp_case(band[0].name, "ice_thickness") == 0) 
      status = nc_def_dim(ncid, "thickness_category", band[0].cat_count, 
			  &dim_depth_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with 'category' definition\n");
  }

  // Define variables
  int dim_1d[1] = { dim_time_id };
  int dim_2d[2] = { dim_height_id, dim_width_id };
  int dim_3d[3] = { dim_time_id, dim_height_id, dim_width_id };
  int dim_4d[4] = { dim_time_id, dim_depth_id, dim_height_id, dim_width_id };
  int dim_time_2d[2] = { dim_time_id, dim_bounds_id };
  for (ii=0; ii<band_count; ii++) {
    if (band[ii].dim_count == 1) {
      status = 
	nc_def_var(ncid, band[ii].name, band[ii].datatype, 1, dim_1d, &var_id);
      if (status != NC_NOERR)
	asfPrintError("Problem with '%s' (1-dimensional) variable definition\n",
		      band[ii].name);
    }
    else if (band[ii].dim_count == 2) {
      if (strcmp_case(band[ii].name, "time_bnds") == 0)
	status = nc_def_var(ncid, "time_bnds", band[ii].datatype, 2, 
			    dim_time_2d, &var_id);
      else
	status = nc_def_var(ncid, band[ii].name, band[ii].datatype, 2, dim_2d, 
			    &var_id);
      if (status != NC_NOERR)
	asfPrintError("Problem with '%s' (2-dimensional) variable definition"
		      "\n", band[ii].name);
    }
    else if (band[ii].dim_count == 3) {
      status = 
	nc_def_var(ncid, band[ii].name, band[ii].datatype, 3, dim_3d, &var_id);
      if (status != NC_NOERR)
	asfPrintError("Problem with '%s' (3-dimensional) variable definition\n",
		      band[ii].name);
    }
    else if (band[ii].dim_count == 4) {
      status = 
	nc_def_var(ncid, band[ii].name, band[ii].datatype, 4, dim_4d, &var_id);
      if (status != NC_NOERR)
	asfPrintError("Problem with '%s' (4-dimensional) variable definition\n",
		      band[ii].name);
    }
    netcdf->var_id[ii] = var_id;
    status = nc_def_var_deflate(ncid, var_id, 0, 1, 6);    
    if (status != NC_NOERR)
      asfPrintError("Problem with '%s' variable deflation\n", band[ii].name);
    
    if (strlen(band[ii].axis) > 0) {
      status = nc_put_att_text(ncid, var_id, "axis", 
			       strlen(band[ii].axis), band[ii].axis);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'axis' attribute definition (%s)\n", 
		      band[ii].name);
    }
    if (strlen(band[ii].cell_methods) > 0) {
      status = nc_put_att_text(ncid, var_id, "cell_methods", 
			       strlen(band[ii].cell_methods), 
			       band[ii].cell_methods);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'cell_methods' attribute definition (%s)\n",
		      band[ii].name);
    }
    if (strlen(band[ii].coordinates) > 0) {
      nc_put_att_text(ncid, var_id, "coordinates", 
		      strlen(band[ii].coordinates), band[ii].coordinates);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'coordinates' attribute definition (%s)\n", 
		      band[ii].name);
    }
    if (strlen(band[ii].grid_mapping) > 0) {
      nc_put_att_text(ncid, var_id, "grid_mapping", 
		      strlen(band[ii].grid_mapping), band[ii].grid_mapping);
      if (status != NC_NOERR)
	asfPrintError("Problem with grid_mapping attribute definition (%s) \n", 
		      band[ii].name);
    }
    if (strlen(band[ii].long_name) > 0) {
      status = nc_put_att_text(ncid, var_id, "long_name", 
		      strlen(band[ii].long_name), band[ii].long_name);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'long_name' attribute definition (%s)\n", 
		      band[ii].name);
    }
    if (strlen(band[ii].references) > 0) {
      status = nc_put_att_text(ncid, var_id, "references", 
			       strlen(band[ii].references), 
			       band[ii].references);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'references' attribute definition (%s)\n", 
		      band[ii].name);
    }
    if (strlen(band[ii].standard_name) > 0) {
      status = nc_put_att_text(ncid, var_id, "standard_name", 
			       strlen(band[ii].standard_name), 
			       band[ii].standard_name);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'standard_name' attribute definition (%s)"
		      "\n", band[ii].name);
    }
    if (strlen(band[ii].units) > 0) {
      status = nc_put_att_text(ncid, var_id, "units", 
			       strlen(band[ii].units), band[ii].units);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'units' attribute definition (%s) \n", 
		      band[ii].name);
    }
    if (strlen(band[ii].units_description) > 0) {
      status = nc_put_att_text(ncid, var_id, "units_description", 
			       strlen(band[ii].units_description), 
			       band[ii].units_description);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'units_description' attribute definition "
		      "(%s)\n",  band[ii].name);
    }
    status = nc_put_att_double(ncid, var_id, "FillValue", NC_DOUBLE, 1, 
		      &band[ii].fill_value);
    if (status != NC_NOERR)
      asfPrintError("Problem with 'FillValue' attribute definition (%s)\n", 
		    band[ii].name);
    if (!ISNAN(band[ii].valid_range[0]) && !ISNAN(band[ii].valid_range[1])) {
      status = nc_put_att_double(ncid, var_id, "valid_range", NC_DOUBLE, 2, 
				 band[ii].valid_range);
      if (status != NC_NOERR)
	asfPrintError("Problem with 'valid range' attribute definition (%s)\n", 
		      band[ii].name);
    }
  }

  // Define global attributes
  strcpy(str, "CF-1.6");
  status = nc_put_att_text(ncid, NC_GLOBAL, "Conventions", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'Conventions' attribute definition\n");
  sprintf(str, "Alaska Satellite Facility");
  status = nc_put_att_text(ncid, NC_GLOBAL, "institution", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'instituion' attribute definition\n");
  if (strcmp_case(mg->sensor, "RGPS MEASURES") == 0)
    sprintf(str, "%s %s image", mg->sensor, mg->bands);
  else
    sprintf(str, "%s %s %s image", mg->sensor, mg->sensor_name, mg->mode);
  status = nc_put_att_text(ncid, NC_GLOBAL, "title", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'title' attribute definition\n");
  strcpy(str, "SAR backcatter image");
  status = nc_put_att_text(ncid, NC_GLOBAL, "source", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'source' attribute definition\n");
  sprintf(str, "%s", mg->basename);
  status = nc_put_att_text(ncid, NC_GLOBAL, "original_file", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'original_file' attribute definition\n");
  if (strcmp_case(mg->sensor, "RGPS MEASURES") == 0)
    strcpy(str, "Copyright Ron Kwok, 2008. MEaSUREs Small-Scale Kinematics of "
	   "Arctic Ocean Sea Ice.");
  else {
    ymd_date ymd;
    hms_time time;
    parse_date(mg->acquisition_date, &ymd, &time);
    if (strcmp_case(mg->sensor, "RSAT-1") == 0)
      sprintf(str, "Copyright Canadian Space Agency, %d", ymd.year);
    else if (strncmp_case(mg->sensor, "ERS", 3) == 0)
      sprintf(str, "Copyright European Space Agency, %d", ymd.year);
    else if (strcmp_case(mg->sensor, "JERS-1") == 0 ||
	     strcmp_case(mg->sensor, "ALOS") == 0)
      sprintf(str, "Copyright Japan Aerospace Exploration Agency , %d", 
	      ymd.year);
  }
  status = nc_put_att_text(ncid, NC_GLOBAL, "comment", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'comment' attribute definition\n");
  strcpy(str, "Documentation available at: www.asf.alaska.edu");
  status = nc_put_att_text(ncid, NC_GLOBAL, "references", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'references' attribute definition\n");
  time_t t;
  struct tm *timeinfo;
  time(&t);
  timeinfo = gmtime(&t);
  sprintf(str, "%s", asctime(timeinfo));
  chomp(str);
  strcat(str, ", UTC: netCDF File created.");
  status = nc_put_att_text(ncid, NC_GLOBAL, "history", strlen(str), str);
  if (status != NC_NOERR)
    asfPrintError("Problem with global 'history' attribute definition\n");

  if (meta_structure) {

    // Metadata  
    int meta_id;
    nc_def_grp(ncid, "metadata", &meta_id);
    
    // Metadata - general block
    nc_meta_str(meta_id, "general_name", "file name", NULL, mg->basename);
    nc_meta_str(meta_id, "general_sensor", "imaging satellite", NULL, 
		mg->sensor);
    nc_meta_str(meta_id, "general_sensor_name", "imaging sensor", NULL, 
		mg->sensor_name);
    nc_meta_str(meta_id, "general_mode", "imaging mode", NULL, mg->mode);
    nc_meta_str(meta_id, "general_processor", "name and version of processor", 
		NULL, mg->processor);
    nc_meta_str(meta_id, "general_data_type", "type of samples (e.g. REAL64)", 
		NULL, data_type2str(mg->data_type));
    nc_meta_str(meta_id, "general_image_data_type", 
		"image data type (e.g. AMPLITUDE_IMAGE)", NULL, 
		image_data_type2str(mg->image_data_type));
    nc_meta_str(meta_id, "general_radiometry", "radiometry (e.g. SIGMA)", NULL, 
		radiometry2str(mg->radiometry));
    nc_meta_str(meta_id, "general_acquisition_date", 
		"acquisition date of the image", NULL, mg->acquisition_date);
    nc_meta_int(meta_id, "general_orbit", "orbit number of image", NULL, 
		&mg->orbit);
    if (mg->orbit_direction == 'A')
      strcpy(str, "Ascending");
    else
      strcpy(str, "Descending");
    nc_meta_str(meta_id, "general_orbit_direction", "orbit direction", NULL, 
		str);
    nc_meta_int(meta_id, "general_frame", "frame number of image", NULL, 
		&mg->frame);
    nc_meta_int(meta_id, "general_band_count", "number of bands in image", NULL,
		&mg->band_count);
    nc_meta_str(meta_id, "general_bands", "bands of the sensor", NULL, 
		mg->bands);
    nc_meta_int(meta_id, "general_line_count", "number of lines in image", NULL,
		&mg->line_count);
    nc_meta_int(meta_id, "general_sample_count", "number of samples in image", 
		NULL, &mg->sample_count);
    nc_meta_int(meta_id, "general_start_line", 
		"first line relative to original image", NULL, &mg->start_line);
    nc_meta_int(meta_id, "general_start_sample", 
		"first sample relative to original image", NULL, 
		&mg->start_sample);
    nc_meta_double(meta_id, "general_x_pixel_size", "range pixel size", "m", 
		   &mg->x_pixel_size);
    nc_meta_double(meta_id, "general_y_pixel_size", "azimuth pixel size", "m", 
		   &mg->y_pixel_size);
    nc_meta_double(meta_id, "general_center_latitude", 
		   "approximate image center latitude", "degrees_north", 
		   &mg->center_latitude);
    nc_meta_double(meta_id, "general_center_longitude",
		   "approximate image center longitude", "degrees_east", 
		   &mg->center_longitude);
    nc_meta_double(meta_id,  "general_re_major", 
		   "major (equator) axis of earth", "m", &mg->re_major);
    nc_meta_double(meta_id, "general_re_minor", "minor (polar) axis of earth", 
		   "m", &mg->re_minor);
    nc_meta_double(meta_id, "general_bit_error_rate", 
		   "fraction of bits which are in error", NULL, 
		   &mg->bit_error_rate);
    nc_meta_int(meta_id, "general_missing_lines", 
		"number of missing lines in data take", NULL, 
		&mg->missing_lines);
    nc_meta_float(meta_id, "general_no_data", 
		  "value indicating no data for a pixel", NULL, &mg->no_data);
    
    if (ms) {
      // Metadata - SAR block
      if (ms->image_type == 'S')
	sprintf(str, "slant range");
      else if (ms->image_type == 'G')
	sprintf(str, "ground range");
      else if (ms->image_type == 'P')
	sprintf(str, "projected");
      else if (ms->image_type == 'R')
	sprintf(str, "georeferenced");    
      nc_meta_str(meta_id, "sar_image_type", "image type", NULL, str);
      if (ms->look_direction == 'R')
	sprintf(str, "right");
      else if (ms->look_direction == 'L')
	sprintf(str, "left");
      nc_meta_str(meta_id, "sar_look_direction", 
		  "SAR satellite look direction", NULL, str);
      nc_meta_int(meta_id, "sar_azimuth_look_count", 
		  "number of looks in azimuth direction", 
		  NULL, &ms->azimuth_look_count);
      nc_meta_int(meta_id, "sar_range_look_count", 
		  "number of looks in range direction", 
		  NULL, &ms->range_look_count);
      nc_meta_int(meta_id, "sar_multilook", "multilooking flag", NULL, 
		  &ms->multilook);
      nc_meta_int(meta_id, "sar_deskewed", "zero doppler deskew flag", NULL, 
		  &ms->deskewed);
      nc_meta_int(meta_id, "sar_original_line_count", 
		  "number of lines in original image", NULL, 
		  &ms->original_line_count);
      nc_meta_int(meta_id, "sar_original_sample_count", 
		  "number of samples in original image", NULL, 
		  &ms->original_sample_count);
      nc_meta_double(meta_id, "sar_line_increment", 
		     "line increment for sampling", NULL, &ms->line_increment);
      nc_meta_double(meta_id, "sar_sample_increment", 
		     "sample increment for sampling", NULL, 
		     &ms->sample_increment);
      nc_meta_double(meta_id, "sar_range_time_per_pixel", 
		     "time per pixel in range", "s", 
		     &ms->range_time_per_pixel);
      nc_meta_double(meta_id, "sar_azimuth_time_per_pixel", 
		     "time per pixel in azimuth", "s", 
		     &ms->azimuth_time_per_pixel);
      nc_meta_double(meta_id, "sar_slant_range_first_pixel", 
		     "slant range to first pixel", "m", 
		     &ms->slant_range_first_pixel);
      nc_meta_double(meta_id, "sar_slant_shift", 
		     "error correction factor in slant range", "m", 
		     &ms->slant_shift);
      nc_meta_double(meta_id, "sar_time_shift", 
		     "error correction factor in time", "s", &ms->time_shift);
      nc_meta_double(meta_id, "sar_wavelength", "SAR carrier wavelength", "m", 
		     &ms->wavelength);
      nc_meta_double(meta_id, "sar_pulse_repetition_frequency", 
		     "pulse repetition frequency", "Hz", &ms->prf);
      nc_meta_double(meta_id, "sar_earth_radius", 
		     "earth radius at scene center", "m", &ms->earth_radius);
      nc_meta_double(meta_id, "sar_satellite_height", 
		     "satellite height from earth's center", "m", 
		     &ms->satellite_height);
      nc_meta_double(meta_id, "sar_range_doppler_centroid", 
		     "range doppler centroid", "Hz", 
		     &ms->range_doppler_coefficients[0]);
      nc_meta_double(meta_id, "sar_range_doppler_linear", 
		     "range doppler per range pixel", "Hz/pixel", 
		     &ms->range_doppler_coefficients[1]);
      nc_meta_double(meta_id, "sar_range_doppler_quadratic", 
		     "range doppler per range pixel square", "Hz/pixel^2", 
		     &ms->range_doppler_coefficients[2]);
      nc_meta_double(meta_id, "sar_azimuth_doppler_centroid", 
		     "azimuth doppler centroid", "Hz", 
		     &ms->azimuth_doppler_coefficients[0]);
      nc_meta_double(meta_id, "sar_azimuth_doppler_linear", 
		     "azimuth doppler per azimuth pixel", "Hz/pixel", 
		     &ms->azimuth_doppler_coefficients[1]);
      nc_meta_double(meta_id, "sar_azimuth_doppler_quadratic", 
		     "azimuth doppler per azimuth per azimuth pixel square", 
		     "Hz/pixel^2", &ms->azimuth_doppler_coefficients[2]);
    }
    
    char tmp[50];
    if (mo) {
      // Metadata - state vector block
      nc_meta_int(meta_id, "orbit_year", "year of image start", NULL, 
		  &mo->year);
      nc_meta_int(meta_id, "orbit_day_of_year", 
		  "day of the year at image start", NULL, &mo->julDay);
      nc_meta_double(meta_id, "orbit_second_of_day", 
		     "second of the day at image start", "seconds", 
		     &mo->second);
      int vector_count = mo->vector_count;
      nc_meta_int(meta_id, "orbit_vector_count", "number of state vectors", 
		  NULL, &vector_count);
      for (ii=0; ii<vector_count; ii++) {
	sprintf(tmp, "orbit_vector[%d]_time", ii+1);
	nc_meta_double(meta_id, tmp, "time relative to image start", "s",
		       &mo->vecs[ii].time);
	sprintf(tmp, "orbit_vector[%d]_position_x", ii+1);
	nc_meta_double(meta_id, tmp, "x coordinate, earth-fixed", "m",
		       &mo->vecs[ii].vec.pos.x);
	sprintf(tmp, "orbit_vector[%d]_position_y", ii+1);
	nc_meta_double(meta_id, tmp, "y coordinate, earth-fixed", "m",
		       &mo->vecs[ii].vec.pos.y);
	sprintf(tmp, "orbit_vector[%d]_position_z", ii+1);
	nc_meta_double(meta_id, tmp, "z coordinate, earth-fixed", "m",
		       &mo->vecs[ii].vec.pos.z);
	sprintf(tmp, "orbit_vector[%d]_velocity_x", ii+1);
	nc_meta_double(meta_id, tmp, "x velocity, earth-fixed", "m/s",
		       &mo->vecs[ii].vec.vel.x);
	sprintf(tmp, "orbit_vector[%d]_velocity_y", ii+1);
	nc_meta_double(meta_id, tmp, "y velocity, earth-fixed", "m/s",
		       &mo->vecs[ii].vec.vel.y);
	sprintf(tmp, "orbit_vector[%d]_velocity_z", ii+1);
	nc_meta_double(meta_id, tmp, "z velocity, earth-fixed", "m/s",
		       &mo->vecs[ii].vec.vel.z);
      }    
    }
>>>>>>> rgps
  }
    
  // Define global attributes
  meta_count = xml_get_children_count(doc, "hdf5.root");
  if (meta_count == 0)
    asfPrintError("No metadata for the root level (hdf5.root) defined!\n");
  strcpy(str, xml_get_string_value(doc, "hdf5.root.conventions"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "Conventions", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.institution"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "institution", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.title"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "title", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.source"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "source", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.original_file"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "original_file", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.comment"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "comment", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.reference"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "reference", strlen(str), str);
  strcpy(str, xml_get_string_value(doc, "hdf5.root.history"));
  if (strcmp_case(str, MAGIC_UNSET_STRING) != 0)
    nc_put_att_text(ncid, NC_GLOBAL, "history", strlen(str), str);

  // Finish off definition block
  nc_enddef(ncid); 
<<<<<<< HEAD

  // Writing data - extra layers first
  nn = 0;
  int nl = meta->general->line_count;
  int ns = meta->general->sample_count;
  long pixel_count = line_count*sample_count;
=======

  if (metadata_xml) {
    // Write ASF metadata to XML file
    char *output_file_name = 
      (char *) MALLOC(sizeof(char)*(strlen(output_file)+5));
    sprintf(output_file_name, "%s.xml", output_file);
    meta_write_xml(meta, output_file_name);
    FREE(output_file_name);
  }

  return netcdf;
}  

void finalize_netcdf_file(int ncid, float *time, float *bounds, 
			  meta_parameters *md)
{
  int var_id;
  float *xgrids, *ygrids;
  int nl = md->general->line_count;
  int ns = md->general->sample_count;
  long pixel_count = md->general->line_count * md->general->sample_count;
  int projected = FALSE;
  if (md->projection && md->projection->type != SCANSAR_PROJECTION)
    projected = TRUE;

  // Extra bands - time
  asfPrintStatus("Storing band 'time' ...\n");
  int status = nc_inq_varid(ncid, "time", &var_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with looking up 'time' variable ID\n");
  status = nc_put_var_float(ncid, var_id, time);
  if (status != NC_NOERR)
    asfPrintError("Problem with writing 'time' variable\n");

  // Extra bands - time bounds (optional)
  if (bounds) {
    asfPrintStatus("Storing band 'time bounds' ...\n");
    status = nc_inq_varid(ncid, "time_bnds", &var_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with looking up 'time_bnds' variable ID\n");
    status = nc_put_var_float(ncid, var_id, bounds);
    if (status != NC_NOERR)
      asfPrintError("Problem with writing 'time_bnds' variable\n");
  }

  // Extra bands - longitude
  int ii, kk;
  double *value, *l, *s, line, sample, lat, lon, first_value;
  quadratic_2d q;
  float *lons = (float *) MALLOC(sizeof(float)*pixel_count);
  if (strcmp_case(md->general->sensor, "RGPS MEASURES") == 0) {
    for (ii=0; ii<nl; ii++)
      for (kk=0; kk<ns; kk++) {
	meta_get_latLon(md, ii, kk, 0.0, &lat, &lon);
	lons[ii*ns+kk] = (float) lon;
      }
  }
  else { // Use quadratic fit
    value = (double *) MALLOC(sizeof(double)*MAX_PTS);
    l = (double *) MALLOC(sizeof(double)*MAX_PTS);
    s = (double *) MALLOC(sizeof(double)*MAX_PTS);
    asfPrintStatus("Generating band 'longitude' ...\n");
    meta_get_latLon(md, 0, 0, 0.0, &lat, &lon);
    if (lon < 0.0)
      first_value = lon + 360.0;
    else
      first_value = lon;
    asfPrintStatus("Calculating grid for quadratic fit ...\n");
    for (ii=0; ii<RES; ii++) {
      for (kk=0; kk<RES; kk++) {
	line = ii * nl / RES;
	sample = kk * ns / RES;
	meta_get_latLon(md, line, sample, 0.0, &lat, &lon);
	l[ii*RES+kk] = line;
	s[ii*RES+kk] = sample;
	if (lon < 0.0)
	  value[ii*RES+kk] = lon + 360.0;
	else
	  value[ii*RES+kk] = lon;
      }
      asfLineMeter(ii, nl);
    }
    q = find_quadratic(value, l, s, MAX_PTS);
    q.A = first_value;
    for (ii=0; ii<nl; ii++) {
      for (kk=0; kk<ns; kk++) {
	lons[ii*ns+kk] = (float)
	  (q.A + q.B*ii + q.C*kk + q.D*ii*ii + q.E*ii*kk + q.F*kk*kk +
	   q.G*ii*ii*kk + q.H*ii*kk*kk + q.I*ii*ii*kk*kk + q.J*ii*ii*ii +
	   q.K*kk*kk*kk) - 360.0;
	if (lons[ii*ns+kk] < -180.0)
	  lons[ii*ns+kk] += 360.0;
      }
      asfLineMeter(ii, nl);
    }
  }
  asfPrintStatus("Storing band 'longitude' ...\n");
  status = nc_inq_varid(ncid, "longitude", &var_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with looking up 'longitude' variable ID\n");
  status = nc_put_var_float(ncid, var_id, lons);
  if (status != NC_NOERR)
    asfPrintError("Problem with writing 'longitude' variable\n");

  // Extra bands - Latitude
  float *lats = (float *) MALLOC(sizeof(float)*pixel_count);
  if (strcmp_case(md->general->sensor, "RGPS MEASURES") == 0) {
    for (ii=0; ii<nl; ii++)
      for (kk=0; kk<ns; kk++) {
	meta_get_latLon(md, ii, kk, 0.0, &lat, &lon);
	lats[ii*ns+kk] = (float) lat;
      }
  }
  else { // Use quadratic fit
    asfPrintStatus("Generating band 'latitude' ...\n");
    meta_get_latLon(md, 0, 0, 0.0, &lat, &lon);
    first_value = lat + 180.0;
    asfPrintStatus("Calculating grid for quadratic fit ...\n");
    for (ii=0; ii<RES; ii++) {
      for (kk=0; kk<RES; kk++) {
	line = ii * nl / RES;
	sample = kk * ns / RES;
	meta_get_latLon(md, line, sample, 0.0, &lat, &lon);
	l[ii*RES+kk] = line;
	s[ii*RES+kk] = sample;
	value[ii*RES+kk] = lat + 180.0;
      }
      asfLineMeter(ii, nl);
    }
    q = find_quadratic(value, l, s, MAX_PTS);
    q.A = first_value;
    for (ii=0; ii<nl; ii++) {
      for (kk=0; kk<ns; kk++) {
	if (md->general->orbit_direction == 'A')
	  lats[(nl-ii-1)*ns+kk] = (float)
	    (q.A + q.B*ii + q.C*kk + q.D*ii*ii + q.E*ii*kk + q.F*kk*kk +
	     q.G*ii*ii*kk + q.H*ii*kk*kk + q.I*ii*ii*kk*kk + q.J*ii*ii*ii +
	     q.K*kk*kk*kk) - 180.0;
	else
	  lats[ii*ns+kk] = (float)
	    (q.A + q.B*ii + q.C*kk + q.D*ii*ii + q.E*ii*kk + q.F*kk*kk +
	     q.G*ii*ii*kk + q.H*ii*kk*kk + q.I*ii*ii*kk*kk + q.J*ii*ii*ii +
	     q.K*kk*kk*kk) - 180.0;
      }
      asfLineMeter(ii, nl);
    }
  }
  asfPrintStatus("Storing band 'latitude' ...\n");
  status = nc_inq_varid(ncid, "latitude", &var_id);
  if (status != NC_NOERR)
    asfPrintError("Problem with looking up 'latitude' variable ID\n");
  status = nc_put_var_float(ncid, var_id, lats);
  if (status != NC_NOERR)
    asfPrintError("Problem with writing 'latitude' variable\nError: %d\n", 
		  status);
>>>>>>> rgps

  if (projected) {
  
    // Extra bands - ygrid
<<<<<<< HEAD
    nn++;
    float *ygrids = (float *) MALLOC(sizeof(float)*pixel_count);
    for (ii=0; ii<line_count; ii++) {
      for (kk=0; kk<sample_count; kk++)
	      ygrids[ii*ns+kk] = 
	        meta->projection->startY + kk*meta->projection->perY;
      asfLineMeter(ii, nl);
    }
    asfPrintStatus("Storing band 'ygrid' ...\n");
    nc_put_var_float(ncid, netcdf->var_id[nn], &ygrids[0]);
    FREE(ygrids);
    
    // Extra bands - xgrid
    nn++;
    float *xgrids = (float *) MALLOC(sizeof(float)*pixel_count);
    for (ii=0; ii<line_count; ii++) {
      for (kk=0; kk<sample_count; kk++) 
	      xgrids[ii*ns+kk] = 
	        meta->projection->startX + kk*meta->projection->perX;
      asfLineMeter(ii, nl);
    }
    asfPrintStatus("Storing band 'xgrid' ...\n");
    nc_put_var_float(ncid, netcdf->var_id[nn], &xgrids[0]);
    FREE(xgrids);
  }

  // Extra bands - longitude
  double line, sample, lat, lon, first_value;
  double *value, *l, *s;
  quadratic_2d q;
  nn++;
  float *lons = NULL;
  if (meta->latlon)
    lons = meta->latlon->lon;
  else {
    value = (double *) MALLOC(sizeof(double)*MAX_PTS);
    l = (double *) MALLOC(sizeof(double)*MAX_PTS);
    s = (double *) MALLOC(sizeof(double)*MAX_PTS);
    lons = (float *) MALLOC(sizeof(float)*pixel_count);
    asfPrintStatus("Generating band 'longitude' ...\n");
    meta_get_latLon(meta, 0, 0, 0.0, &lat, &lon);

    if (lon < 0.0)
      first_value = lon + 360.0;
    else
      first_value = lon;
    asfPrintStatus("Calculating grid for quadratic fit ...\n");
    for (ii=0; ii<RES; ii++) {
      for (kk=0; kk<RES; kk++) {
        line = ii * nl / RES;
        sample = kk * ns / RES;
        meta_get_latLon(meta, line, sample, 0.0, &lat, &lon);
        l[ii*RES+kk] = line;
        s[ii*RES+kk] = sample;
        if (lon < 0.0)
          value[ii*RES+kk] = lon + 360.0;
        else
          value[ii*RES+kk] = lon;
      }
      asfLineMeter(ii, nl);
    }
    q = find_quadratic(value, l, s, MAX_PTS);
    q.A = first_value;
    for (ii=0; ii<nl; ii++) {
      for (kk=0; kk<ns; kk++) {
        lons[ii*ns+kk] = (float)
    (q.A + q.B*ii + q.C*kk + q.D*ii*ii + q.E*ii*kk + q.F*kk*kk +
     q.G*ii*ii*kk + q.H*ii*kk*kk + q.I*ii*ii*kk*kk + q.J*ii*ii*ii +
     q.K*kk*kk*kk) - 360.0;
        if (lons[ii*ns+kk] < -180.0)
          lons[ii*ns+kk] += 360.0;
      }
      asfLineMeter(ii, nl);
    }
    FREE(lons);
  }
  asfPrintStatus("Storing band 'longitude' ...\n");
  nc_put_var_float(ncid, netcdf->var_id[nn], &lons[0]);

  // Extra bands - Latitude
  nn++;
  float *lats;
  if (meta->latlon)
    lats = meta->latlon->lat;
  else {
    lats = (float *) MALLOC(sizeof(float)*pixel_count);
    asfPrintStatus("Generating band 'latitude' ...\n");
    meta_get_latLon(meta, 0, 0, 0.0, &lat, &lon);
    first_value = lat + 180.0;
    asfPrintStatus("Calculating grid for quadratic fit ...\n");
    for (ii=0; ii<RES; ii++) {
      for (kk=0; kk<RES; kk++) {
        line = ii * nl / RES;
        sample = kk * ns / RES;
        meta_get_latLon(meta, line, sample, 0.0, &lat, &lon);
        l[ii*RES+kk] = line;
        s[ii*RES+kk] = sample;
        value[ii*RES+kk] = lat + 180.0;
      }
      asfLineMeter(ii, nl);
    }
    q = find_quadratic(value, l, s, MAX_PTS);
    q.A = first_value;
    for (ii=0; ii<nl; ii++) {
      for (kk=0; kk<ns; kk++) {
        if (meta->general->orbit_direction == 'A')
    lats[(nl-ii-1)*ns+kk] = (float)
      (q.A + q.B*ii + q.C*kk + q.D*ii*ii + q.E*ii*kk + q.F*kk*kk +
       q.G*ii*ii*kk + q.H*ii*kk*kk + q.I*ii*ii*kk*kk + q.J*ii*ii*ii +
       q.K*kk*kk*kk) - 180.0;
        else
    lats[ii*ns+kk] = (float)
      (q.A + q.B*ii + q.C*kk + q.D*ii*ii + q.E*ii*kk + q.F*kk*kk +
       q.G*ii*ii*kk + q.H*ii*kk*kk + q.I*ii*ii*kk*kk + q.J*ii*ii*ii +
       q.K*kk*kk*kk) - 180.0;
      }
      asfLineMeter(ii, nl);
    }
    FREE(lats);
  }
  asfPrintStatus("Storing band 'latitude' ...\n");
  nc_put_var_float(ncid, netcdf->var_id[nn], &lats[0]);

  // Extra bands - Time
  nn++;
  float time = (float) seconds_from_str(meta->general->acquisition_date);
  asfPrintStatus("Storing band 'time' ...\n");
  nc_put_var_float(ncid, netcdf->var_id[nn], &time);
=======
    ygrids = (float *) MALLOC(sizeof(float)*pixel_count);
    for (ii=0; ii<nl; ii++) {
      for (kk=0; kk<ns; kk++)
	ygrids[ii*ns+kk] = 
	  md->projection->startY + kk*md->projection->perY;
    }
    asfPrintStatus("Storing band 'ygrid' ...\n");
    status = nc_inq_varid(ncid, "ygrid", &var_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with looking up 'ygrid' variable ID\n");
    status = nc_put_var_float(ncid, var_id, ygrids);
    if (status != NC_NOERR)
      asfPrintError("Problem with writing 'ygrid' variable\n");
    
    // Extra bands - xgrid
    xgrids = (float *) MALLOC(sizeof(float)*pixel_count);
    for (ii=0; ii<nl; ii++) {
      for (kk=0; kk<ns; kk++) 
	xgrids[ii*ns+kk] = 
	  md->projection->startX + kk*md->projection->perX;
    }
    asfPrintStatus("Storing band 'xgrid' ...\n");
    status = nc_inq_varid(ncid, "xgrid", &var_id);
    if (status != NC_NOERR)
      asfPrintError("Problem with looking up 'xgrid' variable ID\n");
    status = nc_put_var_float(ncid, var_id, xgrids);
    if (status != NC_NOERR)
      asfPrintError("Problem with writing 'xgrid' variable\n");
  }

  // Close file and clean up
  status = nc_close(ncid);
  if (status != NC_NOERR)
    asfPrintError("Could not close netCDF file (%s).\n", nc_strerror(status));
  FREE(lats);
  FREE(lons);
  if (projected) {
    FREE(xgrids);
    FREE(ygrids);
  }
}

void export_netcdf(const char *metadata_file_name, 
		   const char *image_data_file_name,
		   char *output_file_name, char **band_name,
		   int *noutputs,char ***output_names)
{
  int ii, jj, kk, channel, status;
  float *time = (float *) MALLOC(sizeof(float));
  meta_parameters **md = (meta_parameters **) MALLOC(sizeof(meta_parameters *));
  md[0] = meta_read (metadata_file_name); 
  append_ext_if_needed(output_file_name, ".nc", NULL);
  int band_count = md[0]->general->band_count + 3;
  if (md[0]->projection)
    band_count += 2;
  band_t *band = (band_t *) MALLOC(sizeof(band_t)*band_count);
  band_count = md[0]->general->band_count;

  // Image bands
  for (ii=0; ii<band_count; ii++) {
    strcpy(band[ii].name, band_name[ii]);
    strcpy(band[ii].axis, "");
    strcpy(band[ii].cell_methods, "");
    strcpy(band[ii].coordinates, "");
    strcpy(band[ii].grid_mapping, "");
    strcpy(band[ii].long_name, "");
    strcpy(band[ii].references, "");
    strcpy(band[ii].standard_name, "");
    strcpy(band[ii].units, "1");
    strcpy(band[ii].units_description, "unitless");
    strcpy(band[ii].bounds, "time_bnds");
    band[ii].fill_value = 0.0;
    band[ii].valid_range[0] = MAGIC_UNSET_DOUBLE;
    band[ii].valid_range[1] = MAGIC_UNSET_DOUBLE;
    band[ii].time_count = 1;
    band[ii].cat_count = 1;
    band[ii].dim_count = 2;
    band[ii].datatype = NC_FLOAT;
  }

  // Time
  strcpy(band[band_count].name, "time");
  strcpy(band[band_count].axis, "T");
  strcpy(band[band_count].cell_methods, "");
  strcpy(band[band_count].coordinates, "");
  strcpy(band[band_count].grid_mapping, "");
  strcpy(band[band_count].long_name, "serial date");
  strcpy(band[band_count].references, "start and end time of 3-day average");
  strcpy(band[band_count].standard_name, "time");
  strcpy(band[band_count].units, "seconds since 1995-01-01T00:00:00Z");
  strcpy(band[band_count].units_description, "");
  strcpy(band[band_count].bounds, "time_bnds");
  band[band_count].fill_value = 0.0;
  band[band_count].valid_range[0] = MAGIC_UNSET_DOUBLE;
  band[band_count].valid_range[1] = MAGIC_UNSET_DOUBLE;
  band[band_count].time_count = 1;
  band[band_count].cat_count = 1;
  band[band_count].dim_count = 2;
  band[band_count].datatype = NC_FLOAT;

  // Longitude
  strcpy(band[band_count+1].name, "longitude");
  strcpy(band[band_count+1].axis, "");
  strcpy(band[band_count+1].cell_methods, "");
  strcpy(band[band_count+1].coordinates, "");
  strcpy(band[band_count+1].grid_mapping, "");
  strcpy(band[band_count+1].long_name, "longitude");
  strcpy(band[band_count+1].references, "");
  strcpy(band[band_count+1].standard_name, "longitude");
  strcpy(band[band_count+1].units, "degrees_east");
  strcpy(band[band_count+1].units_description, "");
  strcpy(band[band_count+1].bounds, "");
  band[band_count+1].fill_value = -999.0;
  band[band_count+1].valid_range[0] = -180.0;
  band[band_count+1].valid_range[1] = 180.0;
  band[band_count+1].time_count = 1;
  band[band_count+1].cat_count = 1;
  band[band_count+1].dim_count = 2;
  band[band_count+1].datatype = NC_FLOAT;

  // Latitude
  strcpy(band[band_count+2].name, "latitude");
  strcpy(band[band_count+2].axis, "");
  strcpy(band[band_count+2].cell_methods, "");
  strcpy(band[band_count+2].coordinates, "");
  strcpy(band[band_count+2].grid_mapping, "");
  strcpy(band[band_count+2].long_name, "latitude");
  strcpy(band[band_count+2].references, "");
  strcpy(band[band_count+2].standard_name, "latitude");
  strcpy(band[band_count+2].units, "degrees_north");
  strcpy(band[band_count+2].units_description, "");
  strcpy(band[band_count+2].bounds, "");
  band[band_count+2].fill_value = -999.0;
  band[band_count+2].valid_range[0] = -90.0;
  band[band_count+2].valid_range[1] = 90.0;
  band[band_count+2].time_count = 1;
  band[band_count+2].cat_count = 1;
  band[band_count+2].dim_count = 2;
  band[band_count+2].datatype = NC_FLOAT;

  if (md[0]->projection) {
    // XGrid
    strcpy(band[band_count+3].name, "xgrid");
    strcpy(band[band_count+3].axis, "X");
    strcpy(band[band_count+3].cell_methods, "");
    strcpy(band[band_count+3].coordinates, "");
    strcpy(band[band_count+3].grid_mapping, "");
    strcpy(band[band_count+3].long_name, "projection_grid_x_centers");
    strcpy(band[band_count+3].references, "");
    strcpy(band[band_count+3].standard_name, "projection_x_coordinates");
    strcpy(band[band_count+3].units, "meters");
    strcpy(band[band_count+3].units_description, "");
    strcpy(band[band_count+3].bounds, "");
    band[band_count+3].fill_value = MAGIC_UNSET_DOUBLE;
    band[band_count+3].valid_range[0] = MAGIC_UNSET_DOUBLE;
    band[band_count+3].valid_range[1] = MAGIC_UNSET_DOUBLE;
    band[band_count+3].time_count = 1;
    band[band_count+3].cat_count = 1;
    band[band_count+3].dim_count = 2;
    band[band_count+3].datatype = NC_FLOAT;
    
    // YGrid
    strcpy(band[band_count+4].name, "ygrid");
    strcpy(band[band_count+4].axis, "Y");
    strcpy(band[band_count+4].cell_methods, "");
    strcpy(band[band_count+4].coordinates, "");
    strcpy(band[band_count+4].grid_mapping, "");
    strcpy(band[band_count+4].long_name, "projection_grid_y_centers");
    strcpy(band[band_count+4].references, "");
    strcpy(band[band_count+4].standard_name, "projection_y_coordinates");
    strcpy(band[band_count+4].units, "meters");
    strcpy(band[band_count+4].units_description, "");
    strcpy(band[band_count+4].bounds, "");
    band[band_count+4].fill_value = MAGIC_UNSET_DOUBLE;
    band[band_count+4].valid_range[0] = MAGIC_UNSET_DOUBLE;
    band[band_count+4].valid_range[1] = MAGIC_UNSET_DOUBLE;
    band[band_count+4].time_count = 1;
    band[band_count+4].cat_count = 1;
    band[band_count+4].dim_count = 2;
    band[band_count+4].datatype = NC_FLOAT;
  }
  netcdf_t *netcdf = 
    initialize_netcdf_file(output_file_name, band, 1, md[0], TRUE, TRUE);
  int sample_count = md[0]->general->sample_count;
  int line_count = md[0]->general->line_count;
  float *nc = (float *) MALLOC(sizeof(float)*line_count*sample_count);
  FILE *fp = FOPEN(image_data_file_name, "rb");
  float *float_line = (float *) MALLOC(sizeof(float)*sample_count);
>>>>>>> rgps

  // Writing image bands
  float *nc = (float *) MALLOC(sizeof(float)*pixel_count);
  float *float_line = (float *) MALLOC(sizeof(float)*sample_count);
  FILE *fp = FOPEN(data_file_name, "rb");
  char **band_name = extract_band_names(meta->general->bands, band_count);
  int channel;
  for (kk=0; kk<band_count; kk++) {
<<<<<<< HEAD
    for (jj=0; jj<variable_count; jj++) {
      if (strcmp_case(data_set[jj], "LATITUDE") == 0 ||
        strcmp_case(data_set[jj], "LONGITUDE") == 0)
        continue;
      sprintf(xmlStr, "hdf5.data.%s", data_set[jj]);
      strcpy(image_file_name, xml_get_string_value(doc, xmlStr));
      char *p = strstr(image_file_name, ":");
      if (strcmp_case(band_name[kk], p+1) == 0) {
        nn++;
        channel = get_band_number(meta->general->bands, band_count, p+1);
        for (ii=0; ii<line_count; ii++) {
          get_band_float_line(fp, meta, channel, ii, float_line);
          for (hh=0; hh<sample_count; hh++)
            nc[ii*sample_count+hh] = float_line[hh];
          asfLineMeter(ii, line_count);
        }
        asfPrintStatus("Storing band '%s' ...\n", band_name[kk]);
        nc_put_var_float(ncid, netcdf->var_id[nn], &nc[0]);
      }
    }
  }
  
  FCLOSE(fp);
  
  // Close file and clean up
  status = nc_close(ncid);
  if (status != NC_NOERR)
    asfPrintError("Could not close netCDF file (%s).\n", nc_strerror(status));
  FREE(netcdf->var_id);
  FREE(netcdf);
  FREE(nc);
  FREE(float_line);
  meta_free(meta);
  xmlFreeDoc(doc);
  
=======
    for (ii=0; ii<line_count; ii++ ) {
      channel = get_band_number(md[0]->general->bands, band_count, band_name[kk]);
      get_float_line(fp, md[0], ii+channel*line_count, float_line);
      for (jj=0; jj<sample_count; jj++)
	nc[ii*sample_count+jj] = float_line[jj];
      asfLineMeter(ii, md[0]->general->line_count);
    }
    asfPrintStatus("Storing band '%s' ...\n", band_name[kk]);
    status = nc_put_var_float(netcdf->ncid, netcdf->var_id[kk], &nc[0]);
    if (status != NC_NOERR)
      asfPrintError("Problem with writing '%s' variable\n", band_name[kk]);
  }

  if (strcmp_case(md[0]->general->sensor, "RGPS MEASURES") == 0) {
    time[0] = (float) measures2sec(md[0]->general->acquisition_date);
    time[1] = time[0] + 259200.0;
  }
  else
    time[0] = (float) seconds_from_str(md[0]->general->acquisition_date);
  finalize_netcdf_file(netcdf->ncid, time, NULL, md[0]);
  FREE(nc);
  FREE(float_line);
  meta_free(md[0]);
  FREE(md);
  FCLOSE(fp);

>>>>>>> rgps
  *noutputs = 1;
  char **outs = MALLOC(sizeof(char *));
  outs[0] = STRDUP(output_file_name);
<<<<<<< HEAD
  *output_names = outs;
}
=======
  *output_names = outs;  
}

void export_netcdf_list(const char *input_file_list, band_t *band, 
			int band_count, char *output_file_name)
{
  int ii, jj, kk = 0, ll, mm, nn, status;

  // Extracting file information
  int file_count = file_list_count(input_file_list);
  meta_parameters **md = 
    (meta_parameters **) MALLOC(sizeof(meta_parameters *)*file_count);
  char **file = (char **) MALLOC(sizeof(char *)*file_count);
  for (ii=0; ii<file_count; ii++)
    file[ii] = (char *) MALLOC(sizeof(char)*512);

  FILE *fp = FOPEN(input_file_list, "r");
  while (fgets(file[kk], 512, fp)) {
    chomp(file[kk]);
    md[kk] = meta_read(file[kk]);
    kk++;
  }
  FCLOSE(fp);

  // FIXME: metadata check
  // same projection, same dimensions etc.

  // Initialize netCDF file
  append_ext_if_needed(output_file_name, ".nc", NULL);
  netcdf_t *netcdf = initialize_netcdf_file(output_file_name, band, band_count, 
					    md[0], FALSE, FALSE);
  int ncid = netcdf->ncid, var_id, time_count;
  float *time = (float *) MALLOC(sizeof(float)*file_count);
  float *bounds = (float *) MALLOC(sizeof(float)*file_count*2);

  // Write image
  int sample_count = md[0]->general->sample_count;
  int line_count = md[0]->general->line_count;
  int size = line_count*sample_count;
  int bin_count, bin_size;
  float *float_line = (float *) MALLOC(sizeof(float)*sample_count);

  for (mm=0; mm<band_count; mm++) {

    time_count = band[mm].time_count;

    // Write RGPS water mask
    if (strcmp_case(band[mm].name, "water_mask") == 0) {

      int *nc2 = (int *) MALLOC(sizeof(int)*line_count*sample_count);
      for (kk=0; kk<file_count; kk++) {
	if (strcmp_case(md[kk]->general->sensor, "RGPS MEASURES") == 0 &&
	    strcmp_case(md[kk]->general->bands, "WATER MASK") == 0) {
	  asfPrintStatus("Storing band '%s' ...\n", band[mm].name);
	  fp = FOPEN(file[kk], "rb");
	  for (ii=0; ii<line_count; ii++) {
	    get_float_line(fp, md[kk], ii, float_line);
	    for (jj=0; jj<sample_count; jj++)
	      nc2[ii*sample_count+jj] = (int) float_line[jj];
	  }
	  FCLOSE(fp);
	  status = nc_inq_varid (ncid, "water_mask", &var_id);
	  if (status != NC_NOERR)
	    asfPrintError("Problem with looking up 'water_mask' variable\n");
	  status = nc_put_var_int(ncid, var_id, nc2);
	  if (status != NC_NOERR)
	    asfPrintError("Problem with writing 'water_mask' variable\n");
	}
      }
      FREE(nc2);
    }
    else {

      bin_count = band[mm].cat_count;
      bin_size = bin_count*size;
      nn = 0;
      float *nc = (float *) MALLOC(sizeof(float)*line_count*sample_count*
				   time_count*bin_count);
      for (kk=0; kk<file_count; kk++) {
	if (strcmp_case(band[mm].name, md[kk]->general->bands) == 0) {
	  asfPrintStatus("Storing band '%s' ...\n", band[mm].name);
	  fp = FOPEN(file[kk], "rb");
	  for (ll=0; ll<bin_count; ll++) {
	    for (ii=0; ii<line_count; ii++ ) {
	      get_band_float_line(fp, md[kk], ll, ii, float_line);
	      for (jj=0; jj<sample_count; jj++)
		nc[nn*bin_size+ll*size+ii*sample_count+jj] = float_line[jj];
	    }
	  }
	  if (strcmp_case(md[kk]->general->sensor, "RGPS MEASURES") == 0) {
	    bounds[nn*2] = 
	      (float) measures2sec(md[kk]->general->acquisition_date);
	    bounds[(nn*2)+1] = bounds[nn*2] + 259200.0;
	    time[nn] = bounds[nn*2] + 129600.0;
	  }
	  else
	    time[nn] = 
	      (float) seconds_from_str(md[kk]->general->acquisition_date);
	  FCLOSE(fp);
	  nn++;
	}
	status = nc_inq_varid (ncid, band[mm].name, &var_id);
	if (status != NC_NOERR)
	  asfPrintError("Problem with looking up '%s' variable\n", 
			band[mm].name);
	status = nc_put_var_float(ncid, var_id, nc);
	if (status != NC_NOERR)
	  asfPrintError("Problem with writing '%s' variable\n", band[mm].name);
      }
      FREE(nc);
    }
  }
  FREE(float_line);

  // Check for bounds
  int bounds_flag = FALSE;
  for (ii=0; ii<band_count; ii++) {
    if (strcmp_case(band[ii].name, "time_bnds") == 0)
      bounds_flag = TRUE;
  }

  // Finalize netCDF file
  if (bounds_flag)
    finalize_netcdf_file(ncid, time, bounds, md[0]);
  else
    finalize_netcdf_file(ncid, time, NULL, md[0]);

  // Clean up
  FREE(time);
  FREE(bounds);
  for (ii=0; ii<file_count; ii++) {
    FREE(file[ii]);
    meta_free(md[ii]);
  }
  FREE(file);
  FREE(md);
}
>>>>>>> rgps
