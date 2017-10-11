#ifndef __CGDALRASTER_H__
#define __CGDALRASTER_H__

#include <QImage>
#include <QString>
#include <QMatrix>
#include <QColor>

#include <gdal_priv.h>
#include <gdal.h> 
#include "cpl_string.h"

//#include "header.h"


class CGDALRaster {
public:
  GDALDataset*_pData ;
private: ;
 //!< A pointer to data
  int _W, _H ; //!< Width, Height raster
  int _NumBands ; //!< Number of bands in raster
  int _BandsList[3] ; //!< List of bands in raster
  //double _GeoData[6] ; //!< Affine transformation
  QString _ProjectionData ; //!< Projection Name
  int _nPixelSpace ; //!< number of pixels between adjacent pixels
  
  GDALColorTable*_pColorTable ; //!< The Color Table

private: ;
  void Zero() {
    _pData = NULL ;
    _W = 0, _H = 0 ;
    _NumBands = 0 ;
    for( int i = 0 ; i < 3 ; i ++ ) _BandsList[i] = 0 ;
    _ProjectionData = QString() ;
  //  for( int i = 0 ; i < 6 ; i ++ ) _GeoData[i] = 0 ;

    _nPixelSpace = 0 ;
    _pColorTable = NULL ;
  } ;

  void Destroy() {
    if(_pData) {
      //fprintf(stdout,"_pData = %p\n", _pData) ;
      GDALClose(_pData) ;
    } ;

    if(_pColorTable) delete _pColorTable ;
    Zero() ;
  } ;

public: ;
  void set_path(QString const&FileName){ Zero() ; Read(FileName) ; } ;
  CGDALRaster() { Zero() ; } ;
  CGDALRaster(QString const&FileName) { Zero() ; Read(FileName) ; } ;  
  ~CGDALRaster() { Destroy() ; } ;

  bool isNull()const { return !_pData ? true : false ; } ;
  int Width()const { return _W ; }
  int Height()const { return _H ; }

  bool Read( QString const&FileName ) {

    Destroy() ;

    GDALAllRegister() ;
    //_pData = (GDALDataset*)GDALOpen(FileName.toLocal8Bit().data(), GA_Update ) ;
    _pData = (GDALDataset*)GDALOpen(FileName.toLocal8Bit().data(), GA_ReadOnly ) ;
    if( !_pData ) {
      printf("CGDALRaster::Read : Can't open file <%s>\n", FileName.toLocal8Bit().data()) ;
      return false ;
    } ;

    _W = _pData->GetRasterXSize() ;
    _H = _pData->GetRasterYSize() ;

    _ProjectionData.sprintf("%s",_pData->GetProjectionRef()) ;    

    //_pData->GetGeoTransform(_GeoData) ;
    _NumBands = _pData->GetRasterCount() ;
    _BandsList[0] = 1 ; 
    _BandsList[1] = 2 ; 
    _BandsList[2] = 3 ;

    printf("CGDALRaster::Read : Read W = %d : H = %d : NumBands = %d\n", _W, _H, _NumBands ) ;
    printf("CGDALRaster::Read : _ProjectionData = <%s>\n", _ProjectionData.toLocal8Bit().data()) ;

    return true ;
  } ;

QImage GetQImage( QPoint const&Pos, int W, int H ) {

    return GetQImage( Pos, W, H, W, H ) ;
/*
    QImage Img ;

    switch(_NumBands) {
      case 1 : {

        Img = QImage( W, H, QImage::Format_Indexed8) ; 
        Img.setNumColors(256) ;

        // чтение таблицы цветов
        GDALColorTable *pColorTable ;
        pColorTable = (_pData->GetRasterBand(1))->GetColorTable() ;

        for( int i = 0 ; i < 256 ; i ++ ) {
          if ( pColorTable ) {
            const GDALColorEntry *pColor = pColorTable->GetColorEntry(i) ;
            Img.setColor( i, QColor( pColor->c1, pColor->c2, pColor->c3 ).rgb() ) ;
          }
          else 
            Img.setColor( i, QColor(i,i,i).rgb() ) ;     
        } ;

        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), W, H,
                          Img.bits(), Img.width(),Img.height(), GDT_Byte,
                          _NumBands, _BandsList, 0, Img.bytesPerLine(), 0) ;

        Img = Img.convertToFormat(QImage::Format_RGB32);
        
        break ;
      } ;
      case 3 : {
        Img = QImage( W, H, QImage::Format_RGB32) ;    
        Img.fill(0) ; // обнуление изображения

        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), W, H, 
                          Img.bits(), Img.width(),Img.height(), GDT_Byte,
                          _NumBands, _BandsList, 4, Img.bytesPerLine(), 1 ) ;
        break ;
      } ;

      default : {
        printf("CGDALRaster::GetQImage : Incorrect number of bands in raster : _NumBands = %d\n", _NumBands) ;
        break ;
      } ;
    } ;

    return Img ;
*/
  } ;


  QImage GetQImage( QPoint const&Pos, int BaseW, int BaseH, int NewW, int NewH ) {

    QImage Img ;
//   _NumBands = 1;
//   _nPixelSpace = 0 ;
    switch( _NumBands ) {
      case 1 : {
        _nPixelSpace = 0 ;
        Img = QImage( NewW, NewH, QImage::Format_Indexed8) ; 
        Img.setNumColors(256) ;

        // чтение таблицы цветов
        //GDALColorTable *pColorTable ;
        //pColorTable = (_pData->GetRasterBand(1))->GetColorTable() ;

        for( int i = 0 ; i < 256 ; i ++ ) {
          //if ( pColorTable ) {
          //  const GDALColorEntry *pColor = pColorTable->GetColorEntry(i) ;
          // Img.setColor( i, QColor( pColor->c1, pColor->c2, pColor->c3 ).rgb() ) ;
          //}
          //else 
            Img.setColor( i, QColor(i,i,i).rgb( ) ) ;     
        } ;

        //fprintf(stdout,"check\n") ;

        // чтение растровых данных
        _pData->RasterIO ( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH,
                          Img.bits(), Img.width(), Img.height(), GDT_Byte,
                          _NumBands, NULL, _nPixelSpace, Img.bytesPerLine(), 0) ;
//Img.bits - pointer QImage(0,0)
        //Img = Img.convertToFormat(QImage::Format_RGB32);
        
        break ;
      } ;
      case 3 : {
        Img = QImage( NewW, NewH, QImage::Format_RGB32) ;    
        Img.fill(0) ; // обнуление изображения
        _nPixelSpace = 4;
        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH,
                          Img.bits(), Img.width(),Img.height(), GDT_Byte,
                          _NumBands, _BandsList, _nPixelSpace, Img.bytesPerLine(), 1 ) ;
        break ;
      } ;

      default : {
        printf("CGDALRaster::GetQImage : Incorrect number of bands in raster : _NumBands = %d\n", _NumBands) ;
        break ;
      } ;
    } ;

    return Img ;
  } ;
//-----for Krest----
//!get QImage by center point
//   ___W_
//  |     |
// H|  *Pos  
//  |_____|
//   
QImage GetQImageC( QPoint const&Pos, int W, int H ) {

    return GetQImageC( Pos, W, H, W, H ) ;
}

QImage GetQImageC( QPoint const&Pos, int BaseW, int BaseH, int NewW, int NewH ) {

    QImage Img ;

    switch( _NumBands ) {
      case 1 : {

        Img = QImage( NewW, NewH, QImage::Format_Indexed8) ; 
        Img.setNumColors(256) ;

        // чтение таблицы цветов
        //GDALColorTable *pColorTable ;
        //pColorTable = (_pData->GetRasterBand(1))->GetColorTable() ;

        for( int i = 0 ; i < 256 ; i ++ ) {
          //if ( pColorTable ) {
          //  const GDALColorEntry *pColor = pColorTable->GetColorEntry(i) ;
          // Img.setColor( i, QColor( pColor->c1, pColor->c2, pColor->c3 ).rgb() ) ;
          //}
          //else 
            Img.setColor( i, QColor(i,i,i).rgb( ) ) ;     
        } ;

        //fprintf(stdout,"check\n") ;

        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH,
                          Img.bits(), Img.width(), Img.height(), GDT_Byte,
                          _NumBands, NULL, _nPixelSpace, Img.bytesPerLine(), 0) ;

        //Img = Img.convertToFormat(QImage::Format_RGB32);
        
        break ;
      } ;
      case 3 : {
        Img = QImage( NewW, NewH, QImage::Format_RGB32) ;    
        Img.fill(0) ; // обнуление изображения

        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH, 
                          Img.bits(), Img.width(),Img.height(), GDT_Byte,
                          _NumBands, _BandsList, _nPixelSpace, Img.bytesPerLine(), 1 ) ;
        break ;
      } ;

      default : {
        printf("CGDALRaster::GetQImage : Incorrect number of bands in raster : _NumBands = %d\n", _NumBands) ;
        break ;
      } ;
    } ;

    return Img ;
} ;
//-----for Krest----

/* void GetCufftComplex( QPoint const&Pos, int W, int H,cufftComplex*map ) {

    return GetCufftComplex( Pos, W, H, W, H, map ) ;
 } */

/* void GetCufftComplex( QPoint const&Pos, int BaseW, int BaseH, int NewW, int NewH,cufftComplex*map ) {

   // QImage Img ;
   

    switch( _NumBands ) {
      case 1 : {

     //   Img = QImage( NewW, NewH, QImage::Format_Indexed8) ; 
       // Img.setNumColors(256) ;

        // чтение таблицы цветов
        //GDALColorTable *pColorTable ;
        //pColorTable = (_pData->GetRasterBand(1))->GetColorTable() ;

      //  for( int i = 0 ; i < 256 ; i ++ ) {
          //if ( pColorTable ) {
          //  const GDALColorEntry *pColor = pColorTable->GetColorEntry(i) ;
          // Img.setColor( i, QColor( pColor->c1, pColor->c2, pColor->c3 ).rgb() ) ;
          //}
          //else 
        //    Img.setColor( i, QColor(i,i,i).rgb( ) ) ;     
        //} ;

        //fprintf(stdout,"check\n") ;

        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH,
                          map, NewW, NewH, GDT_Byte,
                          _NumBands, NULL, _nPixelSpace,sizeof(cufftComplex)*NewW, 0) ;

        //Img = Img.convertToFormat(QImage::Format_RGB32);
        
        break ;
      } ;
      case 3 : {
        Img = QImage( NewW, NewH, QImage::Format_RGB32) ;    
        Img.fill(0) ; // обнуление изображения

        // чтение растровых данных
        _pData->RasterIO( GF_Read, Pos.x(), Pos.y(), BaseW, BaseH, 
                          map, NewW, NewH, GDT_Byte,
                          _NumBands, _BandsList, _nPixelSpace,sizeof(cufftComplex)*NewW, 1 ) ;
        break ;
      } ;

      default : {
        printf("CGDALRaster::GetQImage : Incorrect number of bands in raster : _NumBands = %d\n", _NumBands) ;
        break ;
      } ;
    } ;

   // return Img ;
  } ;
*/
  bool CreateGeoTIFF( QString const&filename, int W, int H, 
                      /*double*pGeoData,*/ QString const&PrjName ) {

    Destroy() ;
  GDALDriver *pdriver;

    GDALAllRegister() ;
    GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("GTiff" ) ;

    // создание доп. файла
    char **papszOptions = NULL;      
    papszOptions = CSLSetNameValue( papszOptions, "TFW", "YES" ) ;
char block_size_str[100];
char **pcreate_str=0;
    _W = W ;
    _H = H ;

    // создаем цветной GeoTiff
    _NumBands = 3 ;
    _BandsList[0] = 1 ; 
    _BandsList[1] = 2 ; 
    _BandsList[2] = 3 ;
    _nPixelSpace = 4 ;

    int*pBandList = _BandsList ;
    int nBandSpace = 1 ;

    QString filenameTiff = filename + ".tif" ;

				sprintf(block_size_str,"%d",128);
				printf("block_size_str=%s\n",block_size_str);
				// задание размера блока
				pcreate_str=CSLSetNameValue(pcreate_str,"BLOCKXSIZE",block_size_str);
				pcreate_str=CSLSetNameValue(pcreate_str,"BLOCKYSIZE",block_size_str);
				
				//pcreate_str=CSLSetNameValue(pcreate_str,"BIGTIFF","YES");
				pcreate_str=CSLSetNameValue(pcreate_str,"TILED","YES");
				pcreate_str=CSLSetNameValue(pcreate_str,"PHOTOMETRIC","RGB");
//pimg=pdriver->Create(name_out.toLocal8Bit().data(),W,H,1,type_data,pcreate_str);
//    _pData = pDriver->Create( filenameTiff.toLocal8Bit().data(), W, H, _NumBands, GDT_Byte, papszOptions ) ;
    _pData = pDriver->Create( filenameTiff.toLocal8Bit().data(), W, H, _NumBands, GDT_Byte, pcreate_str ) ;
    if(!_pData){
      printf("CGDALRaster::CreateGeoTIFF : Can't open file <%s>\n", filenameTiff.toLocal8Bit().data()) ;
      return false ;
    } ;

    //if(pGeoData) SetGeoTransform(pGeoData) ;
    if(!PrjName.isNull()) SetProjectionData(PrjName) ;

    {
      QString filenamePrj = filename + ".prj" ;
      FILE*fid = fopen( filenamePrj.toLocal8Bit().data(), "wt") ;
      if(fid)
        fprintf(fid, "%s", PrjName.toLocal8Bit().data()) ;
      fclose(fid) ;
    } ;

    return true ;
  } ;

  bool CreateGeoTIFF_GrayScale( QString const&filename, int W, int H,
                           /*double*pGeoData,*/ QString const&PrjName ) {

    Destroy() ;

    GDALAllRegister() ;
    GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("GTiff" ) ;

    // создание доп. файла
    char **papszOptions = NULL;
    papszOptions = CSLSetNameValue( papszOptions, "TFW", "YES" ) ;

    _W = W ;
    _H = H ;

    // создаем чб GeoTiff
    _NumBands = 1 ;
    _BandsList[0] = 1 ;
    _nPixelSpace = 1 ;

    int*pBandList = _BandsList ;
    int nPixelSpace = _nPixelSpace ;
    int nBandSpace = 1 ;

    QString filenameTiff = filename + ".tif" ;

    _pData = pDriver->Create( filenameTiff.toLocal8Bit().data(), W, H, _NumBands, GDT_Byte, papszOptions ) ;
    if(!_pData){
      printf("CGDALRaster::CreateGeoTIFF : Can't open file <%s>\n", filenameTiff.toLocal8Bit().data()) ;
      return false ;
    } ;

   // if(pGeoData) SetGeoTransform(pGeoData) ;
   // if(!PrjName.isNull()) SetProjectionData(PrjName) ;

  /*  {
      QString filenamePrj = filename + ".prj" ;
      FILE*fid = fopen( filenamePrj.toLocal8Bit().data(), "wt") ;
      if(fid)
        fprintf(fid, "%s", PrjName.toLocal8Bit().data()) ;
      fclose(fid) ;
    } ;*/

  //  GDALRasterBand*poBand = _pData->GetRasterBand(1) ;

    //GDALColorInterp InterpType = poBand->GetColorInterpretation() ;
    //fprintf(stdout,"InterpType = %d\n", InterpType) ;
    //GDALColorTable*poCT = poBand->GetColorTable() ;
    //fprintf(stdout,"poCT = %p\n", poCT) ;

   /* {
      GDALColorEntry colorYellow ;
      colorYellow.c1 = 255 ; colorYellow.c2 = 255 ; colorYellow.c3 = 0 ; colorYellow.c4 = 0 ;

      GDALColorEntry colorGreen ;
      colorGreen.c1 = 0 ; colorGreen.c2 = 255 ; colorGreen.c3 = 0 ; colorGreen.c4 = 0 ;

      GDALColorEntry colorWhite ;
      colorWhite.c1 = 255 ; colorWhite.c2 = 255 ; colorWhite.c3 = 255 ; colorWhite.c4 = 0 ;

      _pColorTable = new GDALColorTable ;

      // установим два цвета:желтый и зеленый
      _pColorTable->SetColorEntry(127, &colorYellow) ;
      _pColorTable->SetColorEntry(64, &colorGreen) ;
      _pColorTable->SetColorEntry(255, &colorWhite) ;

      poBand->SetColorTable(_pColorTable) ;
    } ;*/

    return true ;
  } ;


  bool Write( QPoint const&Pos, QImage const&Img ) { return Write(Pos.x(), Pos.y(), Img) ; } ;
  
  bool Write( int Xoffset, int Yoffset, QImage const&Img ) {
    QImage ImgRGB(Img.width(),Img.height(),QImage::Format_RGB32);
   // ImgRGB.convertToFormat(QImage::Format_RGB32,Qt::MonoOnly);
    int color;
/*        ImgRGB.setNumColors(256) ;

        for( int i = 0 ; i < 256 ; i ++ ) {
            ImgRGB.setColor( i, QColor(i,i,i).rgb( ) ) ;  
            puts("ya tutWrite");
        } ;   */
    for(int i = 0; i < Img.height();++i)
       for(int j = 0; j < Img.width();++j) {
          color=qGray(Img.pixel(j,i));
        //  puts("ya tut");
        //  color=Img.pixelIndex(j,i);
          ImgRGB.setPixel(j,i,qRgb(color,color,color));    
//          ImgRGB.setPixel(j,i,color);     
       }
       

    if(!_pData) return false ;

    int W = Img.width() ;
    int H = Img.height() ;

    int NumBands = _NumBands ;
    int*pBandList = _BandsList ;
    int nPixelSpace = _nPixelSpace ;

    NumBands = 3;
    nPixelSpace = 4;
    pBandList = NULL;
    
    int nBandSpace = 1 ;

    CPLErr ErrStatus=_pData->RasterIO(GF_Write,Xoffset,Yoffset,W,H,(void*)ImgRGB.bits(),W,H,GDT_Byte,
                                      NumBands,pBandList,nPixelSpace,ImgRGB.bytesPerLine(),nBandSpace);
    bool isSuccess =  true ;
    if((int)ErrStatus != 0) {
      printf("CGDALRaster::Write : ErrStatus = %d\n", (int)ErrStatus) ;
      isSuccess = false ;
    } ;

    return isSuccess ;
  } ;

  void SetGeoTransform(double *pData) { 
    _pData->SetGeoTransform(pData) ; 

    //for( int i = 0 ; i < 6 ; i ++ )
      //_GeoData[i] = pData[i] ;
  } ;

  void SetProjectionData(QString const&ProjectionData) { 
    _pData->SetProjection( ProjectionData.toLocal8Bit().data() ) ;
    _ProjectionData = ProjectionData ;
  } ;

  static bool SaveGeoBMP( QImage const&Img, QString const&FileName, /*double*pGeoData */) {

    GDALAllRegister() ;
    //GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("GTiff" ) ;
    GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("BMP" ) ;
    

    /*printf("pGeoData :\n") ;
    for( int i = 0 ; i < 6 ; i ++ ) {
      printf("%lf \t", pGeoData[i]) ;
    } ;
    printf("\n") ;*/

    GDALDataset *poDstDS;       
    char **papszOptions = NULL;      
    papszOptions = CSLSetNameValue( papszOptions, "WORLDFILE","YES" ) ;
    //papszOptions = CSLSetNameValue( papszOptions, "TILED","YES" ) ;
    //papszOptions = CSLSetNameValue( papszOptions, "TFW","YES" ) ;
    //WORLDFILE=YES
    
    int W = Img.width() ;
    int H = Img.height() ;

    int NumBands = 0 ;
    int BandsList[3] = { 1, 2, 3 } ;
    int nPixelSpace = 0 ;
    int*pBandList = BandsList ;
    int nBandSpace = 0 ;
    GDALColorTable ColorTable ;

    switch(Img.format()) {

      case QImage::Format_Indexed8 : {

        printf("QImage::Format_Indexed8\n") ;

        QVector<QRgb> ImgColorTable = Img.colorTable() ;

        for( int i = 0 ; i < ImgColorTable.size() ; i ++ ) {
          QRgb iRGB = ImgColorTable[i] ;
          GDALColorEntry iColorEntry ;
          iColorEntry.c1 = (short)qRed(iRGB) ;
          iColorEntry.c2 = (short)qGreen(iRGB) ;
          iColorEntry.c3 = (short)qBlue(iRGB) ;
          iColorEntry.c4 = 255 ;
          ColorTable.SetColorEntry( i, &iColorEntry) ;
        } ;

        NumBands = 1 ;
        nPixelSpace = 1 ;
        //pBandList = NULL ;
        nBandSpace = 0 ;
        break ;
      } ;

      case QImage::Format_RGB32 : {

        printf("QImage::Format_RGB32\n") ;
        
        NumBands = 3 ;
        nPixelSpace = 4 ;
        nBandSpace = 1 ;
        break ;
      } ;

      default : {
        printf("CGDALRaster::CreateGeoBMP : Incorrect QImage format\n") ;
        return  false ;
      } ;

    } ;

    poDstDS = pDriver->Create( FileName.toLocal8Bit().data(), W, H, NumBands, GDT_Byte, papszOptions ) ;
    if(!poDstDS){
      printf("CGDALRaster::CreateGeoBMP : Can't open file <%s>\n", FileName.toLocal8Bit().data()) ;
      return false ;
    } ;

    if(NumBands == 1) (poDstDS->GetRasterBand(1))->SetColorTable(&ColorTable) ;
    //CPLErr ErrStatusSetGeoData = poDstDS->SetGeoTransform(pGeoData) ;
    printf("ErrStatusSetGeoData = %d : CE_None = %d\n", ErrStatusSetGeoData, CE_None) ;

    CPLErr ErrStatus = poDstDS->RasterIO( GF_Write , 0, 0, W, H, (void*)Img.bits(), W, H, GDT_Byte,
                                          NumBands, pBandList, nPixelSpace, Img.bytesPerLine(), nBandSpace ) ;

    if((int)ErrStatus != 0)
      printf("CGDALRaster::CreateGeoBMP : ErrStatus = %d\n", (int)ErrStatus) ;

    {
      //double TestGeoData[6] ;
      //CPLErr ErrStatusTestGeo = poDstDS->GetGeoTransform(TestGeoData) ;
     // printf("ErrStatusTestGeo = %d : CE_None = %d\n", ErrStatusTestGeo, CE_None) ;

      /*printf("TestGeoData :\n") ;
      for( int i = 0 ; i < 6 ; i ++ ) {
        printf("%lf \t", TestGeoData[i]) ;
      } ;
      printf("\n") ; */
    } ;

    GDALClose(poDstDS) ;
    return (int)ErrStatus == 0 ? true : false ;
  } ;


  static bool SaveGeoTIFF( QImage const&Img, QString const&FileName, 
                           /*double*pGeoData*/, QString const&PrjName ) {
    if(Img.isNull()) {                           
      printf("CGDALRaster::SaveGeoTIFF : Img.isNull()\n") ;
      return  false ;
    } ;

    GDALAllRegister() ;
    GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("GTiff" ) ;
    //GDALDriver*pDriver =(GDALDriver*) GDALGetDriverByName("HFA" ) ;

    GDALDataset *poDstDS;       
    char **papszOptions = NULL;      
    //papszOptions = CSLSetNameValue( papszOptions, "WORLDFILE","YES" ) ;
    //papszOptions = CSLSetNameValue( papszOptions, "TILED","YES" ) ;

    // создание доп. файла
    papszOptions = CSLSetNameValue( papszOptions, "TFW", "YES" ) ;

    //papszOptions = CSLSetNameValue( papszOptions, "WORLDFILE","YES" ) ;
    //WORLDFILE=YES

    int W = Img.width() ;
    int H = Img.height() ;

    int NumBands = 0 ;
    int BandsList[3] = { 1, 2, 3 } ;
    int nPixelSpace = 0 ;
    int*pBandList = BandsList ;
    int nBandSpace = 0 ;
    GDALColorTable ColorTable ;

    switch(Img.format()) {

      case QImage::Format_Indexed8 : {

        printf("QImage::Format_Indexed8\n") ;

        QVector<QRgb> ImgColorTable = Img.colorTable() ;

        for( int i = 0 ; i < ImgColorTable.size() ; i ++ ) {
          QRgb iRGB = ImgColorTable[i] ;
          GDALColorEntry iColorEntry ;
          iColorEntry.c1 = (short)qRed(iRGB) ;
          iColorEntry.c2 = (short)qGreen(iRGB) ;
          iColorEntry.c3 = (short)qBlue(iRGB) ;
          iColorEntry.c4 = 255 ;
          ColorTable.SetColorEntry( i, &iColorEntry) ;
        } ;

        NumBands = 1 ;
        nPixelSpace = 1 ;
        //pBandList = NULL ;
        nBandSpace = 0 ;
        break ;
      } ;

      case QImage::Format_RGB32 : {

        printf("QImage::Format_RGB32\n") ;
        
        NumBands = 3 ;
        nPixelSpace = 4 ;
        nBandSpace = 1 ;
        break ;
      } ;

      default : {
        printf("CGDALRaster::SaveGeoTIFF : Incorrect QImage format\n") ;
        return  false ;
      } ;

    } ;

    QString TiffFileName = FileName + ".tif" ;
    poDstDS = pDriver->Create( TiffFileName.toLocal8Bit().data(), W, H, NumBands, GDT_Byte, papszOptions ) ;
    if(!poDstDS){
      printf("CGDALRaster::SaveGeoTIFF : Can't open file <%s>\n", FileName.toLocal8Bit().data()) ;
      return false ;
    } ;

    if(NumBands == 1) (poDstDS->GetRasterBand(1))->SetColorTable(&ColorTable) ;

    //if(pGeoData) poDstDS->SetGeoTransform(pGeoData) ;
    if(!PrjName.isNull()) poDstDS->SetProjection (PrjName.toLocal8Bit().data()) ;

    CPLErr ErrStatus = poDstDS->RasterIO( GF_Write , 0, 0, W, H, (void*)Img.bits(), W, H, GDT_Byte,
                                          NumBands, pBandList, nPixelSpace, Img.bytesPerLine(), nBandSpace ) ;

    if((int)ErrStatus != 0)
      printf("CGDALRaster::SaveGeoTIFF : ErrStatus = %d\n", (int)ErrStatus) ;
    else {
      QString PrjFileName = FileName + ".prj" ;
      FILE*fid = fopen( PrjFileName.toLocal8Bit().data(), "wt") ;
      fprintf(fid, "%s", PrjName.toLocal8Bit().data()) ;
      fclose(fid) ;
    } ;

    GDALClose(poDstDS) ;
    return (int)ErrStatus == 0 ? true : false ;
  } ;

  /*void GetGeoTransform( double*pOutData )const { 
    for( int i = 0 ; i < 6 ; i ++ ) 
      pOutData[i] = _GeoData[i] ;
  } ;*/

  QString GetProjectionData()const { return _ProjectionData ; } ;

  // Transform from: Pixels (Line,Column) -> Geographic coordinates (X,Y)
  /*QMatrix GetTransformMatrix()const { 
    double m11 = _GeoData[1] ;
    double m12 = _GeoData[4] ;
    double m21 = _GeoData[2] ;
    double m22 = _GeoData[5] ;
    
    double dx = _GeoData[0] ;
    double dy = _GeoData[3] ;
    
    return QMatrix( m11, m12, m21, m22, dx, dy) ; 
  } ;*/

  //QSizeF GetPixelSize()const { return QSizeF( fabs(_GeoData[1]), fabs(_GeoData[5])) ; }

 void create_overviews(QString name) {
     int numCount = (_pData->GetRasterBand(_pData->GetRasterCount()))->GetOverviewCount(); 
     QFile ovrfile(name+".ovr");
        if(!ovrfile.exists()&&numCount==0){
            int anOverviewList[3] = { 4, 8, 16 };
            _pData->BuildOverviews("NEAREST",3,anOverviewList,0,NULL,NULL,NULL);
        }  
 }

} ;


#endif
