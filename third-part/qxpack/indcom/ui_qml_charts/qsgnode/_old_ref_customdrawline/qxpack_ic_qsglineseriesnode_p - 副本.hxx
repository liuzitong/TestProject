#ifndef QXPACK_IC_QSGLINESERIESNODE_P_HXX
#define QXPACK_IC_QSGLINESERIESNODE_P_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QColor>
#include <QSGSimpleMaterial>
#include <QList>
#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QtGlobal>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//                   the line series node
//
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcQSGLineSeriesNode : public QSGGeometryNode {
public:
    // ========================================================================
    // the line material component
    // ========================================================================
    struct QXPACK_IC_HIDDEN Material {
          enum LineType { LineType_Solid = 0 };
          QColor color; float smooth, size;  LineType ln_type;
          qreal  rdrc_x, rdrc_y, rdrc_width, rdrc_height;
          qreal  rdwc_width, rdwc_height;

          explicit Material() {
              color = QColor("black"); smooth = -1; size = -1; ln_type = LineType_Solid;
              rdrc_x = rdrc_y = 0; rdrc_width = rdrc_height = -1;
              rdwc_width = rdwc_height = -1;
          }

          Material( float sz, float sm, const QColor &cl, LineType ln_t )
          {
             size   = ( sz >= 0.0f ? sz : 0.0f );
             smooth = ( sm >= 0.0f ? sm : 0.0f );
             color  = cl; ln_type = ln_t;
          }

          // convert the render item rect to GL position
          void  setRenderRect( const QRectF &bd, const QSizeF &win_sz )
          {
              rdrc_x = bd.x(); rdrc_y = bd.y(); rdrc_width = bd.width(); rdrc_height = bd.height();
              rdwc_width = win_sz.width(); rdwc_height = win_sz.height();
          }

          bool operator == ( const Material &other ) const
          {
           return (  color == other.color && ln_type == other.ln_type &&
                     qFuzzyCompare( smooth, other.smooth ) &&
                     qFuzzyCompare( size,   other.size   ) &&
                     qFuzzyCompare( rdrc_x, other.rdrc_x  ) &&
                     qFuzzyCompare( rdrc_y, other.rdrc_y  ) &&
                     qFuzzyCompare( rdrc_width,  other.rdrc_width  ) &&
                     qFuzzyCompare( rdrc_height, other.rdrc_height ) &&
                     qFuzzyCompare( rdwc_width,  other.rdwc_width  ) &&
                     qFuzzyCompare( rdwc_height, other.rdwc_height )
                   );
           }
    };

private:
    QSGGeometry  m_geom;
    QSGSimpleMaterial<Material>  *m_render_mater;
protected:
    QList<QPointF>    clipData ( const QList<QPointF> &, const QRectF & );
    void              normData (       QList<QPointF> &, const QRectF & );
    void              scaleData(       QList<QPointF> &, const QRectF & );
    QList<QPointF>    makeContour( const QList<QPointF> &, const double &sz );
public :
    explicit IcQSGLineSeriesNode( const Material &mater );
    virtual ~IcQSGLineSeriesNode() Q_DECL_OVERRIDE;
    bool  qsg_updateMaterial( const Material &mater );
    void  qsg_updateGeometry( const QRectF &bounds, const QList<QPointF> &samples, const QRectF &vw_port, const Material &mater );
};


}

#endif
