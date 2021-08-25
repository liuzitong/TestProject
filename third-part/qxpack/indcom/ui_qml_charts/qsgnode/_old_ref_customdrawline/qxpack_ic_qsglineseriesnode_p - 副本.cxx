#ifndef QXPACK_IC_QSGLINESERIESNODE_P_CXX
#define QXPACK_IC_QSGLINESERIESNODE_P_CXX

#include "qxpack_ic_qsglineseriesnode_p.hxx"
#include "qxpack/indcom/sys/qxpack_ic_geom2dalgor.hxx"
#include <QByteArray>
#include <QSGSimpleMaterialShader>

namespace QxPack {
namespace IcQSGLineSeriesNodeSpc {

// ////////////////////////////////////////////////////////////////////////////
//  the vertext structure
// ////////////////////////////////////////////////////////////////////////////
struct QXPACK_IC_HIDDEN LnSerVertex {
    float x,y,t;
    inline void set(float xx, float yy, float tt ) { x = xx; y = yy; t = tt; }
};

static const QSGGeometry::AttributeSet &gBuild_attributes()
{
    static QSGGeometry::Attribute attr[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true), // (x,y) position value
        QSGGeometry::Attribute::create(1, 1, GL_FLOAT )       // t value
    };
    static QSGGeometry::AttributeSet set = { 2, 3 * sizeof(float), attr };
    return set;
}

// ============================================================================
// find out viewport by data list
// ============================================================================
static QRectF  gFindViewPort( const QList<QPointF> &dat )
{
   if ( dat.isEmpty()) { return QRectF(); }

   auto c_itr = dat.constBegin();
   const QPointF &c_pt = *( c_itr ++ );
   qreal min_x = c_pt.x(); qreal max_x = c_pt.x();
   qreal min_y = c_pt.y(); qreal max_y = c_pt.y();

   while ( c_itr != dat.constEnd()) {
       const QPointF &pt = *( c_itr ++ );
       if ( min_x > pt.x()) { min_x = pt.x(); }
       if ( max_x < pt.x()) { max_x = pt.x(); }
       if ( min_y > pt.y()) { min_y = pt.y(); }
       if ( max_y < pt.y()) { max_y = pt.y(); }
   }

   return QRectF( min_x, max_y, max_x - min_x, max_y - min_y );
}


// ////////////////////////////////////////////////////////////////////////////
//                      line series material shader
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  LnSerShader : public QSGSimpleMaterialShader<IcQSGLineSeriesNode::Material> {
    QSG_DECLARE_SIMPLE_SHADER( LnSerShader, IcQSGLineSeriesNode::Material )
public :
    LnSerShader() : id_color(-1), id_smooth(-1), id_size(-1)
   {
        // setShaderSourceFile read data, so vertexShader() and fragmentShader() will return the text
        setShaderSourceFile( QOpenGLShader::Vertex,   ":/qxpack/indcom/ui_qml_charts/shader/lineseries.vsh");
        setShaderSourceFile( QOpenGLShader::Fragment, ":/qxpack/indcom/ui_qml_charts/shader/lineseries.fsh");
    }

    // ========================================================================
    // NOTE: "pos" and "t" will appear in vertex shader
    //     them's order just follown the member IcQuickLineSeriesVertex
    // ========================================================================
    QList<QByteArray> attributes() const override {
        return QList<QByteArray>() << "attr_pos" << "attr_type";
    }

    void updateState( const IcQSGLineSeriesNode::Material *m, const IcQSGLineSeriesNode::Material * ) override {
        // --------------------------------------------------------------------
        // NOTE: this function used to upload values to OpenGL, here can use
        // upload texture and bind texture for shader use.
        // --------------------------------------------------------------------
        program()->setUniformValue( id_color,  m->color  ); // upload uniform values
        program()->setUniformValue( id_smooth, m->smooth );
        program()->setUniformValue( id_size,   m->size   );
    }

    void resolveUniforms() override {
        id_smooth = program()->uniformLocation("line_smooth");
        id_size   = program()->uniformLocation("line_size"  );
        id_color  = program()->uniformLocation("line_color" );
    }
private:
    int id_color,  id_smooth, id_size;
};

}

// ////////////////////////////////////////////////////////////////////////////
// QSG Line Series Node
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor ( create it in QSG thread )
// ============================================================================
IcQSGLineSeriesNode :: IcQSGLineSeriesNode(  const Material &mater )
    : m_geom( IcQSGLineSeriesNodeSpc::gBuild_attributes(), 0 )
{
    // setup the geometry, draw mode trangle strip
    setGeometry( & m_geom );
    m_geom.setDrawingMode( GL_QUAD_STRIP ); // use this draw mode to draw trangle rectangle

    // setup the material shader, used to draw the trangle strip.
    m_render_mater = IcQSGLineSeriesNodeSpc::LnSerShader::createMaterial();
    *( m_render_mater->state()) = mater; // copy data for render thread use.
    m_render_mater->setFlag( QSGMaterial::Blending ); // means require GL_BLEND flag

    // install the material, and take it's ownership.
    setMaterial( m_render_mater  ); // internal called markDirty( QSGNode::DirtyMaterial ) ???
    setFlag( OwnsMaterial );
    markDirty( QSGNode::DirtyMaterial );
}

// ============================================================================
// dtor
// ============================================================================
IcQSGLineSeriesNode :: ~IcQSGLineSeriesNode( )
{

}

// ============================================================================
// clip the data, make all data is about in viewport
// ============================================================================
QList<QPointF>          IcQSGLineSeriesNode :: clipData( const QList<QPointF> &dat, const QRectF &vw_prt )
{
    // ------------------------------------------------------------------------
    // if viewport is not valid, need not to do clip
    // ------------------------------------------------------------------------
    if ( ! vw_prt.isValid() ) { return dat; }

    // ------------------------------------------------------------------------
    // clip data, drop other data that not in the viewport
    // ------------------------------------------------------------------------
    int sz = dat.size();

    if ( sz > 1 ) {
        QList<QPointF> fix_dat;
        qreal vw_min = vw_prt.left() - vw_prt.width() * 0.125;
        qreal vw_max = vw_prt.right()+ vw_prt.width() * 0.125;
        auto c_itr = dat.constBegin();
        auto n_itr = dat.constBegin() + 1;
        bool is_c_pt_in_vw = false; bool is_n_pt_in_vw = false;
        while ( n_itr != dat.constEnd()) {
            const QPointF &c_pt = *( c_itr ++ );
            const QPointF &n_pt = *( n_itr ++ );
            is_c_pt_in_vw = ( c_pt.x() >= vw_min && c_pt.x() <= vw_max );
            is_n_pt_in_vw = ( n_pt.x() >= vw_min && n_pt.x() <= vw_max );
            if ( is_c_pt_in_vw || is_n_pt_in_vw ) { fix_dat.append( c_pt );}

            is_c_pt_in_vw = is_n_pt_in_vw; is_n_pt_in_vw = false;
        }
        const QPointF &e_pt = *( c_itr );
        if ( is_c_pt_in_vw || ( e_pt.x() >= vw_min && e_pt.x() <= vw_max ))
        { fix_dat.append( e_pt ); }

        return fix_dat;
    } else {
        return dat;
    }
}

// ============================================================================
// normalize the data
// ============================================================================
void                    IcQSGLineSeriesNode :: normData( QList<QPointF> &dat, const QRectF &vw_prt )
{
    if ( ! vw_prt.isValid() ) { return; }

    double inv_width = 1.0 / vw_prt.width();
    double inv_height= 1.0 / vw_prt.height();
    auto itr = dat.begin();
    while ( itr != dat.end()) {
        QPointF &pt = *( itr ++ );
        pt.rx() = ( pt.x() - vw_prt.x()) * inv_width;
        // RectF x,y is a top-left, here convert the math coord. to UI coord.
        pt.ry() = ( vw_prt.y() - pt.y()) * inv_height; // so use vw_prt.y() - pt.y()
    }
}

// ============================================================================
// bound the data
// ============================================================================
void                   IcQSGLineSeriesNode :: scaleData ( QList<QPointF> &dat, const QRectF &bounds )
{
    double x = bounds.x();     double y = bounds.y();
    double w = bounds.width(); double h = bounds.height();
    auto itr = dat.begin();
    while( itr != dat.end()) {
        QPointF &pt = *( itr ++ );
        double vy = y + ( pt.y() ) * h;
        double vx = x + ( pt.x() ) * w;
        pt.rx() = vx; pt.ry() = vy;
    }
}

// ============================================================================
// make polygon line contour
// ============================================================================
QList<QPointF>          IcQSGLineSeriesNode :: makeContour( const QList<QPointF> &dat, const double &tk )
{
//    switch( m_render_mater->state()->ln_type ) {
//    case IcQSGLineSeriesNode::Material::LineType_Solid : return IcGeom2DAlgor::makeSoildPolLnContour( dat, tk, false );
//    }
    return IcGeom2DAlgor::makeSoildPolLnContour( dat, tk, false );
}

// ============================================================================
// change the material value
// ============================================================================
bool                    IcQSGLineSeriesNode :: qsg_updateMaterial( const Material &mater )
{
    if ( !( * m_render_mater->state() == mater )) {
        *( m_render_mater->state()) = mater;
        markDirty( QSGNode::DirtyMaterial );  // mark a flag, so render thread can re-render it using new material
        return true;
    } else {
        return false;
    }
}

// ============================================================================
// draw the geometry
// ============================================================================
void                    IcQSGLineSeriesNode :: qsg_updateGeometry( const QRectF &bounds, const QList<QPointF> &samples , const QRectF &vw_port, const Material &mater )
{
    // ------------------------------------------------------------------------
    // avoid empty data
    // ------------------------------------------------------------------------
    if ( samples.isEmpty()) {
        m_geom.allocate( 0 ); markDirty( QSGNode::DirtyGeometry );
        return;
    }

    // ------------------------------------------------------------------------
    // if no viewport, assume all data should be displayed. so here we need to
    // find out viewport
    // ------------------------------------------------------------------------
    QRectF vw_prt = vw_port;
    if ( !vw_prt.isValid()) {
        vw_prt = IcQSGLineSeriesNodeSpc::gFindViewPort( samples );
    }

    QList<QPointF> dat = clipData( samples, vw_prt );

    if ( ! dat.isEmpty() ) {
        normData ( dat, vw_prt );
        scaleData( dat, bounds );
        dat = makeContour( dat, double( mater.size ) + 0.375 );
        int dat_sz = dat.size();
        m_geom.allocate( dat_sz );

        auto *v = static_cast< IcQSGLineSeriesNodeSpc::LnSerVertex *>( m_geom.vertexData() );
        auto c_itr = dat.constBegin(); int ofv = 0;
        for ( int i = 0, i_max = dat_sz / 4; i < i_max; i ++, c_itr += 4, ofv += 4 ) {
            const QPointF &pt1 = *( c_itr    ); const QPointF &pt2 = *( c_itr + 1 );
            const QPointF &pt3 = *( c_itr + 2); const QPointF &pt4 = *( c_itr + 3 );
            v[ ofv     ].set( float(pt1.x()), float(pt1.y()), 0 );
            v[ ofv + 1 ].set( float(pt2.x()), float(pt2.y()), 1 );
            v[ ofv + 2 ].set( float(pt3.x()), float(pt3.y()), 0 );
            v[ ofv + 3 ].set( float(pt4.x()), float(pt4.y()), 1 );
        }
        for ( int i = 0, i_max = dat_sz % 4; i < i_max; i ++, ofv ++ ) {
            const QPointF &pt = *( c_itr ++ );
            v[ ofv ].set( float(pt.x()), float(pt.y()), (ofv & 1));
        }

    } else {
        m_geom.allocate(0);
    }

    markDirty( QSGNode::DirtyGeometry );
}



}
#endif
