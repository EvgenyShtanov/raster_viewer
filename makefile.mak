
include $(MAKE_FOR_SYSTEM)
	
base_dir=$(BASE_DIR)
name_exe=interior_orientation_test.exe
##############################################

source_mylib_aux_dir = $(MYLIB_DIR)/auxdir
source_mylib_cuda_dir = $(MYLIB_DIR)/cuda
source_mylib_devraster_dir = $(MYLIB_DIR)/devraster
source_mylib_math_la_dir = $(MYLIB_DIR)/math/LA
source_mylib_matching_dir = $(MYLIB_DIR)/matching
source_mylib_types_dir = $(MYLIB_DIR)/types
source_mylib_camera_dir = $(MYLIB_DIR)/camera
source_mylib_geometry_dir = $(MYLIB_DIR)/math/geometry
source_mylib_video_analysis_dir = $(MYLIB_DIR)/video_analysis

##############################################

	
# ���������� � ��������� �������
source_dir=$(base_dir)/source
source_shot_dir=$(base_dir)/shot
source_geom_dir=$(SHARED_MODULE_DIR)/geom
source_raster_dir=$(SHARED_MODULE_DIR)/raster
# ���������� � ��������� ������� ������ ������ � tiff-�������
source_tiff_dir=$(SHARED_MODULE_DIR)/source_tiff
# ���������� � ��������� ������� � �����
source_network_dir=$(SHARED_MODULE_DIR)/source_network
# ���������� � �������� ������� �������������������� ������
source_photogrammetry_dir=$(SHARED_MODULE_DIR)/source_photogrammetry
# ���������� � ��������� ������� ������ ������ � ����������������� ��
source_cartographic_sc_dir=$(SHARED_MODULE_DIR)/cartographic_sc
# ��������� � ��������� ������� ��� ������ � ������� ���-����������
source_gis_dir=$(SHARED_MODULE_DIR)/source_gis
# ���������� � ��������� ������� ��� ������ � ����������������� ��������
source_multi_shot_dir=$(SHARED_MODULE_DIR)/source_multi_shot
source_equiv_shot_dir=$(SHARED_MODULE_DIR)/source_equiv_shot
#
source_module_link=$(SHARED_MODULE_DIR)/source_module_link
# ���������� � ��������� ������� ��� ������ � �������� �������
source_relief_dir=$(SHARED_MODULE_DIR)/source_relief
# ���������� � ��������� ������� ��� ������ � ����
source_ksoi_dir=$(SHARED_MODULE_DIR)/source_ksoi
# ���������� � h-������� ������� ����

source_graphic_item_image=$(base_dir)/graphic_item_image

source_header_form_dir=$(base_dir)/form_h
# ���������� � ui-������� ������� ����
source_form_dir=$(base_dir)/form_ui

# ���������� ��� ��������� ������
obj_dir=$(base_dir)/obj
# ���������� ��� moc-������
moc_dir=$(base_dir)/moc
# ���������� ��� ������������ �����
proc_dir=$(base_dir)/exe


# ���������� � ������������� �������
library_source_dir=$(base_dir)/library_source
library_obj_dir=$(base_dir)/library_obj

# ���������� � ������������� �������
cpp_options_include=$(opt_inc) "$(NIITP_CPP)/include" \
							$(opt_inc) "$(TIFF_DIR)/include" \
							$(opt_inc) "$(GDAL_DIR)/include" \
							$(opt_inc) "$(source_geom_dir)" \
							$(opt_inc) "$(source_cartographic_sc_dir)" \
							$(opt_inc) "$(source_dir)" \
							$(opt_inc) "$(source_header_form_dir)"

#							$(opt_inc) "$(source_raster_dir)" \
#							
#							$(opt_inc) "$(source_shot_dir)" \
#							$(opt_inc) "$(source_geom_dir)" \
#							$(opt_inc) "$(source_network_dir)" \
#							$(opt_inc) "$(source_photogrammetry_dir)" \
#							$(opt_inc) "$(source_tiff_dir)" \
#							$(opt_inc) "$(base_dir)/lib" \
#							$(opt_inc) "$(GIS_DIR)/include" \
#							$(opt_inc) "$(source_gis_dir)" \
#							$(opt_inc) "$(source_multi_shot_dir)" \
#							$(opt_inc) "$(source_header_form_dir)"  \
#							$(opt_inc) "$(source_cartographic_sc_dir)" \
#							$(opt_inc) "$(source_ksoi_dir)" \
#							$(opt_inc) "$(source_equiv_shot_dir)" \
#							$(opt_inc) "$(source_module_link)" \
#							$(opt_inc) "$(source_relief_dir)"

##############################################
mylib_options_include =	$(opt_inc) "$(source_mylib_aux_dir)" \
						$(opt_inc) "$(source_mylib_cuda_dir)" \
						$(opt_inc) "$(source_mylib_devraster_dir)" \
						$(opt_inc) "$(source_mylib_math_la_dir)" \
						$(opt_inc) "$(source_mylib_matching_dir)" \
						$(opt_inc) "$(source_mylib_types_dir)" \
						$(opt_inc) "$(source_mylib_camera_dir)" \
						$(opt_inc) "$(source_mylib_geometry_dir)" \
						$(opt_inc) "$(source_mylib_video_analysis_dir)"

ifdef USE_MKL
cpp_options_include+=\
						$(opt_inc) "$(MKLLIB_DIR)/include"
endif


ifdef USE_IPP
cpp_options_include+=\
						$(opt_inc) "$(IPPLIB_DIR)/include"
endif

ifdef USE_OPENCV
cpp_options_include+=\
						$(opt_inc) "$(CVLIB_DIR)/CV/include" \
						$(opt_inc) "$(CVLIB_DIR)/OtherLibs/_Ipl/include" \
						$(opt_inc) "$(CVLIB_DIR)/cxcore/include" \
						$(opt_inc) "$(CVLIB_DIR)/otherlibs/highgui"
endif

#mylib_options_include+=\
#						$(opt_inc) "$(MYLIB_DIR)\image" \
#						$(opt_inc) "$(MYLIB_DIR)"
						
##############################################							
 
# ������� ������ 
.SUFFIXES :

# h-����� � ������� ��� ����
h_file_widget=$(source_header_form_dir)/form_mainwindow_krest.h

# ��������� ����� �� ����������� moc-������
obj_file=\
	$(obj_dir)/main.$(ext_obj)\
	$(obj_dir)/Krest_window.$(ext_obj)\
	$(obj_dir)/krestview.$(ext_obj)\
	$(obj_dir)/krest.$(ext_obj)\
	$(obj_dir)/RubberRect.$(ext_obj)\
	$(obj_dir)/Krest_scene.$(ext_obj)\
	$(obj_dir)/cthread.$(ext_obj)
#	$(obj_dir)/transformation.$(ext_obj)

#	$(obj_dir)/auxqimage.$(ext_obj) \

#	$(obj_dir)/cgdalraster.$(ext_obj)
	

# ��������� ����� ���������� �� moc-������						
obj_moc_file=\
	$(obj_dir)/krest_window_moc.$(ext_obj)\
	$(obj_dir)/krest_scene_moc.$(ext_obj)\
	$(obj_dir)/cthread_moc.$(ext_obj)
#	$(obj_dir)/transformation_moc.$(ext_obj)\

# ������������ ������
library_obj_file=

# ��������� ����� � cuda-�����
obj_file_cuda=

all:
	$(MAKE) $(opt_make_f) makefile.mak clean
	$(MAKE) $(opt_make_f) makefile.mak program

clean:
#	$(command_del) $(base_dir)$(SD)obj$(SD)*.$(ext_obj)
#	$(command_del) $(base_dir)$(SD)library_obj$(SD)*.$(ext_obj)

	$(command_del) $(base_dir)$(SD)moc$(SD)*moc.cpp
	$(command_del) $(obj_file)
	$(command_del) $(obj_moc_file)
	$(command_del) $(library_obj_file)
	$(command_del) $(obj_file_cuda)
	$(command_del) $(proc_dir)/$(name_exe)

# �������� ���� "���������� ����������"
program :  $(h_file_widget) $(proc_dir)/$(name_exe)

# �������� h-������ �� ������ ui-������
$(source_header_form_dir)/form_mainwindow_krest.h : $(source_form_dir)/form_mainwindow_krest.ui
	$(UIC) -o $@ $?

# �������� moc-������
$(moc_dir)/krest_window_moc.cpp : $(source_dir)/Krest_window.h
	$(MOC) -o $@ $?

$(moc_dir)/krest_scene_moc.cpp : $(source_dir)/Krest_scene.h
	$(MOC) -o $@ $?

$(moc_dir)/cthread_moc.cpp : $(source_dir)/cthread.h
	$(MOC) -o $@ $?

#$(moc_dir)/transformation_moc.cpp : $(source_dir)/transformation.h
#	$(MOC) -o $@ $?

#kompiliatia moc failov

$(obj_dir)/krest_window_moc.$(ext_obj) : $(moc_dir)/krest_window_moc.cpp
	$(command_comp)

$(obj_dir)/krest_scene_moc.$(ext_obj) : $(moc_dir)/krest_scene_moc.cpp
	$(command_comp)

$(obj_dir)/cthread_moc.$(ext_obj) : $(moc_dir)/cthread_moc.cpp
	$(command_comp)

#$(obj_dir)/transformation_moc.$(ext_obj) : $(moc_dir)/transformation_moc.cpp
#	$(command_comp)
# �������� �����

#$(obj_dir)/auxqimage.$(ext_obj) : $(source_mylib_aux_dir)/auxqimage.cpp 
#	$(command_comp)

#$(obj_dir)/cgdalraster.$(ext_obj) : $(source_dir)/cgdalraster.cpp
#	$(command_comp)

$(obj_dir)/Krest_window.$(ext_obj) : $(source_dir)/Krest_window.cpp
	$(command_comp)	

$(obj_dir)/main.$(ext_obj) : $(source_dir)/main.cpp
	$(command_comp)

$(obj_dir)/krestview.$(ext_obj) : $(source_dir)/krestview.cpp
	$(command_comp)

$(obj_dir)/RubberRect.$(ext_obj) : $(source_dir)/RubberRect.cpp
	$(command_comp)	

$(obj_dir)/krest.$(ext_obj) : $(source_dir)/krest.cpp
	$(command_comp)	

$(obj_dir)/Krest_scene.$(ext_obj) : $(source_dir)/Krest_scene.cpp
	$(command_comp)

$(obj_dir)/cthread.$(ext_obj) : $(source_dir)/cthread.cpp
	$(command_comp)	

#$(obj_dir)/transformation.$(ext_obj) : $(source_dir)/transformation.cpp
#	$(command_comp)

#$(obj_dir)/cu_func.$(ext_obj) : $(source_dir)/cu_func.cu
#	$(command_comp_cuda)

#$(obj_dir)/cthread_moc.$(ext_obj) : $(moc_dir)/cthread_moc.cpp
#	$(command_comp)	
	

######################################################

#  ����������� �������������� �������
#include $(source_mylib_correlator_dir)/correlator.mak
#include $(source_mylib_math_la_dir)/la.mak
#include $(base_dir)/makefile_lib.mak
#include  $(source_cartographic_sc_dir)/cartographic_sc.mak

include $(source_graphic_item_image)/graphic_item_image.mak

# �������� ���������� � ������� ����������
$(proc_dir)/$(name_exe)  : $(obj_file) $(obj_moc_file) $(library_obj_file) $(obj_file_cuda)
	$(command_link)  $(obj_file) $(obj_moc_file)   $(library_obj_file) $(obj_file_cuda) $(library)

