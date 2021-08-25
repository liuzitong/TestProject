#ifndef QXPACK_IC_QSGVALUEAXISNODE_P_CXX
#define QXPACK_IC_QSGVALUEAXISNODE_P_CXX

#include "qxpack_ic_qsgvalueaxisnode_p.hxx"
#include "qxpack/indcom/sys/qxpack_ic_geom2dalgor.hxx"
#include <QByteArray>
#include <QSGSimpleMaterialShader>
#include <QtMath>
#include <QPainter>
#include <QImage>

namespace QxPack {
namespace IcQSGValueAxisNodeSpc {

// ////////////////////////////////////////////////////////////////////////////
//                      the vertext structure
// ////////////////////////////////////////////////////////////////////////////
struct QXPACK_IC_HIDDEN LnVertex {
    float x,y,t; // position and a user type value
    inline void set( float xx, float yy, float tt ) { x = xx; y = yy; t = tt; }
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

// ////////////////////////////////////////////////////////////////////////////
// the ticks information
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  TickInfo {
private:
    qreal    m_vrg_min,  m_vrg_max;    // tick visble value range
    qreal    m_brg_min,  m_brg_max;    // base line value range
    qreal    m_btk_step, m_mj_tk_step; // base tick step ( the minimal value step ),
    qreal    m_nr_min_tk_idx, m_nr_max_tk_idx; // nearest tick index in view port,
                                               // NOTE: the tick index is begin with parameter's minimal value.
    qintptr  m_btk_cnt;

public :
    explicit TickInfo( const qreal &vw_min, const qreal &vw_max, const IcQSGValueAxisNode::LnParam &ln_par )
    {
        // [ m_vrg_min, m_vrg_max ] is used for ticks ( visible for user )
        if ( vw_min <= vw_max ) {
            m_vrg_min = ( ln_par.min() <= vw_min ? vw_min : ( ln_par.min() <= vw_max ? ln_par.min() : vw_max + 1.0 ));
            m_vrg_max = ( ln_par.max() >= vw_max ? vw_max : ( ln_par.max() >= vw_min ? ln_par.max() : vw_min - 1.0 ));
        } else {
            m_vrg_min = ln_par.min();
            m_vrg_max = ln_par.max();
        }

        // [ m_brg_min, m_brg_max ] is used for base line
        if ( vw_min <= vw_max ) {
            m_brg_min = ( ln_par.blnMin() <= vw_min ? vw_min : ( ln_par.blnMin() <= vw_max ? ln_par.blnMin() : vw_max + 1.0 ));
            m_brg_max = ( ln_par.blnMax() >= vw_max ? vw_max : ( ln_par.blnMax() >= vw_min ? ln_par.blnMax() : vw_min - 1.0 ));
        } else {
            m_brg_max = ln_par.blnMax();
            m_brg_min = ln_par.blnMin();
        }

        m_btk_step = m_mj_tk_step = 0; m_btk_cnt = -1;
        m_nr_max_tk_idx = m_nr_max_tk_idx = -1;
        if ( isValid() ) {
            m_mj_tk_step = ( ln_par.max() - ln_par.min() ) / ( ln_par.majorTickCnt() - 1 );
            m_btk_cnt  = ( ln_par.majorTickCnt() - 1 ) * ln_par.minorTickCnt() + ln_par.majorTickCnt();
            m_btk_step = m_mj_tk_step / ( ln_par.minorTickCnt() + 1 );

            std::modf(( m_vrg_min - ln_par.min() ) / m_btk_step, &m_nr_min_tk_idx );
            std::modf(( m_vrg_max - ln_par.min() ) / m_btk_step, &m_nr_max_tk_idx );
            if ( m_nr_min_tk_idx * m_btk_step > m_vrg_min ) { m_nr_min_tk_idx --; }
            if ( m_nr_max_tk_idx * m_btk_step < m_vrg_max ) { m_nr_max_tk_idx ++; }
        }
    }

    inline bool  isValid()      const { return m_brg_min < m_brg_max; }
    inline bool  hasTickInVrg() const { return m_nr_max_tk_idx >= m_nr_min_tk_idx; }

    inline const qreal &  baseTickStep() const { return m_btk_step; }
    inline const qreal &  vrgMin() const { return m_vrg_min; }
    inline const qreal &  vrgMax() const { return m_vrg_max; }
    inline const qreal &  brgMin() const { return m_brg_min; }
    inline const qreal &  brgMax() const { return m_brg_max; }
    inline const qreal &  nrMinTickIdx() const { return m_nr_min_tk_idx; }
    inline const qreal &  nrMaxTickIdx() const { return m_nr_max_tk_idx; }
    inline qint64  nrMinTickIdxInt() const { return qint64( m_nr_min_tk_idx ); }
    inline qint64  nrMaxTickIdxInt() const { return qint64( m_nr_max_tk_idx ); }
};


// ////////////////////////////////////////////////////////////////////////////
//                      line series material shader
// ////////////////////////////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  LnShader : public QSGSimpleMaterialShader<IcQSGValueAxisNode::Material> {
    QSG_DECLARE_SIMPLE_SHADER( LnShader, IcQSGValueAxisNode::Material )
public :
    LnShader() : id_color(-1), id_smooth(-1), id_size(-1)
    {
        // setShaderSourceFile read data, so vertexShader() and fragmentShader() will return the text
        setShaderSourceFile( QOpenGLShader::Vertex,   ":/qxpack/indcom/ui_qml_charts/shader/valueaxis.vsh");
        setShaderSourceFile( QOpenGLShader::Fragment, ":/qxpack/indcom/ui_qml_charts/shader/valueaxis.fsh");
    }

    // ========================================================================
    // NOTE: "pos" and "t" will appear in vertex shader
    //     them's order just follown the member IcQuickLineSeriesVertex
    // ========================================================================
    QList<QByteArray> attributes() const override
    {
        return QList<QByteArray>() << "attr_pos" << "attr_type";
    }

    void updateState( const IcQSGValueAxisNode::Material *m, const IcQSGValueAxisNode::Material *) override
    {
        // --------------------------------------------------------------------
        // NOTE: this function used to upload values to OpenGL, here can use
        // upload texture and bind texture for shader use.
        // --------------------------------------------------------------------
        program()->setUniformValue( id_color,  m->color  ); // upload uniform values
        program()->setUniformValue( id_smooth, m->smooth );
        program()->setUniformValue( id_size,   m->size   );
    }

    void resolveUniforms() override
    {
        id_smooth = program()->uniformLocation("line_smooth");
        id_size   = program()->uniformLocation("line_size"  );
        id_color  = program()->uniformLocation("line_color" );
    }
private:
    int id_color, id_smooth, id_size;
};

}

// ////////////////////////////////////////////////////////////////////////////
// QSG Line Series Node
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor ( create it in QSG thread )
// ============================================================================
IcQSGValueAxisNode :: IcQSGValueAxisNode(  const Material &mater )
    : m_geom( IcQSGValueAxisNodeSpc::gBuild_attributes(), 0 )
{
    // setup the geometry, draw mode QUADS
    setGeometry( & m_geom );
    m_geom.setDrawingMode( GL_QUADS );

    // setup the material shader, used to draw the trangle strip.
    m_render_mater = IcQSGValueAxisNodeSpc::LnShader::createMaterial();
    *( m_render_mater->state()) = mater; // copy data for render thread use.
    m_render_mater->setFlag( QSGMaterial::Blending );  // means require GL_BLEND flag

    // install the material, and take it's ownership.
    setMaterial( m_render_mater  );
    setFlag( OwnsMaterial );         // 'm_render_mater' lifecycle is managed by this QSGNODE
    markDirty( QSGNode::DirtyMaterial );
}

// ============================================================================
// dtor
// ============================================================================
IcQSGValueAxisNode :: ~IcQSGValueAxisNode( )
{
}

// ============================================================================
// just redraw the axis by new material
// ============================================================================
bool                    IcQSGValueAxisNode :: qsg_updateMaterial( const Material &mater )
{
    if ( !( * m_render_mater->state() == mater )) {
        *( m_render_mater->state()) = mater;
        markDirty( QSGNode::DirtyMaterial );
        return true;
    } else {
        return false;
    }
}

// ============================================================================
// draw the geometry by vertical direction
// ============================================================================
void                    IcQSGValueAxisNode :: qsg_updGeomVert(
    const QRectF &bounds, const LnParam &ln_par, const QRectF &vw_port, const Material &mater, IcQuickLblLayoutSrc *lbl_layout
) {
    IcQSGValueAxisNodeSpc::TickInfo tick_info( vw_port.y(), vw_port.bottom(), ln_par );

    // ------------------------------------------------------------------------
    // clear all data if tick_info is invalid
    // ------------------------------------------------------------------------
    if ( ! tick_info.isValid() ) {
        m_geom.allocate( 0 ); markDirty( QSGNode::DirtyGeometry );
        if ( lbl_layout != nullptr ) { lbl_layout->invalidate(); }
        return;
    }

    // ------------------------------------------------------------------------
    // generate all ticks and base line
    // ------------------------------------------------------------------------
    qreal  inv_rg_sz = 1.0 / ( vw_port.isValid() ? vw_port.height() : ln_par.max() - ln_par.min() );
    qreal  bd_ofv = bounds.y(), bd_sz = bounds.height();
    float  px_pos = float( bounds.width() * 0.5 );

    // pre-alloc point in geometry buffer
    if ( tick_info.hasTickInVrg()) {
        int  tick_sz = int( tick_info.nrMaxTickIdxInt() - tick_info.nrMinTickIdxInt() + 1 );
        m_geom.allocate( ( ln_par.isShowBln() ? 4 : 0 ) + 4 * tick_sz );
    } else {
        m_geom.allocate( ln_par.isShowBln() ? 4 : 0 );
    }
    if ( ! tick_info.hasTickInVrg() && ! ln_par.isShowBln()) { return; }

    IcQSGValueAxisNodeSpc::LnVertex *vtx = static_cast< IcQSGValueAxisNodeSpc::LnVertex *>( m_geom.vertexData() );
    float px_fix = ( float(0.0005) + mater.size ) * float(0.5);
    int vtx_ofv = 0;

    // generate the horizontal base line points
    if ( ln_par.isShowBln() ) {   // NOTE: axis Y should be map to math coord.
        qreal rg_ofv  = ( vw_port.isValid() ? vw_port.y() : ln_par.blnMin() );
        qreal pct_pos = ( tick_info.brgMin() - rg_ofv ) * inv_rg_sz;

        qreal sc_pos  = ( bd_sz - pct_pos * bd_sz ) + bd_ofv;
        vtx[0].set ( px_pos - px_fix, float( sc_pos ), 0 );
        vtx[1].set ( px_pos + px_fix, float( sc_pos ), 1 );

        pct_pos = ( tick_info.brgMax() - rg_ofv ) * inv_rg_sz;
        sc_pos  = ( bd_sz - pct_pos * bd_sz ) + bd_ofv;
        vtx[2].set ( px_pos + px_fix, float( sc_pos ), 1 );
        vtx[3].set ( px_pos - px_fix, float( sc_pos ), 0 );

        vtx_ofv = 4;
    }

    // generate the points for ticks
    if ( tick_info.hasTickInVrg()) {
        qint64 nr_min_idx = tick_info.nrMinTickIdxInt(); // nearest minimum tick index
        qint64 nr_max_idx = tick_info.nrMaxTickIdxInt(); // nearest maxmum  tick index
        qreal  rg_ofv = ( vw_port.isValid() ? vw_port.y() : ln_par.min() );

        qint64 mj_f_rd_v  = ln_par.minorTickCnt() + 1;
        qint64 mn_hf_rd_v = ( ln_par.minorHintStep() > 0 ? ln_par.minorHintStep() : mj_f_rd_v );
        qint64 mj_f_cntr  = nr_min_idx % mj_f_rd_v;
        qint64 mn_hf_cntr = nr_min_idx % mn_hf_rd_v;
        if ( mj_f_cntr  == 0 ) { mj_f_cntr  = mj_f_rd_v; }
        if ( mn_hf_cntr == 0 ) { mn_hf_cntr = mn_hf_rd_v; }

        // setup label layout data
        if ( lbl_layout != nullptr ) {
            lbl_layout->init (
                nr_min_idx, nr_max_idx, rg_ofv, 1.0 / inv_rg_sz, tick_info.baseTickStep(), ln_par.min(),
                mj_f_rd_v, mn_hf_rd_v
            );
        }

        // a direction trick for later use ( reduce the 'if' sentence )
        float  tk_ccw_sz = 0, tk_cw_sz = 0;
        float *tk_ccw_ptr = &tk_ccw_sz, *tk_cw_ptr = &tk_cw_sz;
        switch( ln_par.tickDir() ) {
        case IcQSGValueAxisNode::LnParam::TickDir_CCW : tk_cw_ptr  = tk_ccw_ptr; break;
        case IcQSGValueAxisNode::LnParam::TickDir_CW  : tk_ccw_ptr = tk_cw_ptr;  break;
        default: break;
        }

        for ( qint64 i = nr_min_idx; i <= nr_max_idx; i ++, mj_f_cntr ++, mn_hf_cntr ++ ) {

           // overwite the tick size by ccw and cw, here used a pointer trick tech.
           // Assume CCW and CW ticks are used.
           *tk_ccw_ptr = float( ln_par.minorSize()); *tk_cw_ptr = *tk_ccw_ptr;
           if ( mn_hf_cntr == mn_hf_rd_v ) {
              *tk_ccw_ptr = float( ln_par.minorStepSize()); *tk_cw_ptr = *tk_ccw_ptr;
              mn_hf_cntr = 0;
           }
           if ( mj_f_cntr == mj_f_rd_v ) {
              *tk_ccw_ptr = float( ln_par.majorSize()); *tk_cw_ptr = *tk_ccw_ptr;
              mj_f_cntr = 0;
           }

           // translate the math point to item coord.
           double pct_pos = ( tick_info.baseTickStep() * qreal(i) + ln_par.min() - rg_ofv ) * inv_rg_sz;
           double sc_pos  = ( bd_sz - pct_pos * bd_sz ) + bd_ofv;

           if ( pct_pos >= 0 && pct_pos <= 1.0 ) {
               vtx[vtx_ofv    ].set ( px_pos - tk_ccw_sz, float( sc_pos ) - px_fix, 0 );
               vtx[vtx_ofv + 1].set ( px_pos - tk_ccw_sz, float( sc_pos ) + px_fix, 1 );
               vtx[vtx_ofv + 2].set ( px_pos + tk_cw_sz,  float( sc_pos ) + px_fix, 1 );
               vtx[vtx_ofv + 3].set ( px_pos + tk_cw_sz,  float( sc_pos ) - px_fix, 0 );
           } else {
               vtx[ vtx_ofv ].set( 0, 0, 0 );
               vtx[ vtx_ofv + 1 ].set( 0, 0, 1 );
               vtx[ vtx_ofv + 2 ].set( 0, 0, 1 );
               vtx[ vtx_ofv + 3 ].set( 0, 0, 0 );
           }
           vtx_ofv += 4;
       }
    } else {
        if ( lbl_layout != nullptr ) { lbl_layout->invalidate(); }
    }

    markDirty( QSGNode::DirtyGeometry );
}

// ============================================================================
// draw the geometry by horizontal direction
// ============================================================================
void                    IcQSGValueAxisNode :: qsg_updGeomHori(
    const QRectF &bounds, const LnParam &ln_par, const QRectF &vw_port, const Material &mater, IcQuickLblLayoutSrc *lbl_layout
) {
    IcQSGValueAxisNodeSpc::TickInfo tick_info( vw_port.x(), vw_port.right(), ln_par );

    // ------------------------------------------------------------------------
    // clear all data if tick_info is invalid
    // ------------------------------------------------------------------------
    if ( ! tick_info.isValid() ) {
        m_geom.allocate( 0 ); markDirty( QSGNode::DirtyGeometry );
        if ( lbl_layout != nullptr ) { lbl_layout->invalidate(); }
        return;
    }

    // ------------------------------------------------------------------------
    // generate all ticks and base line
    // ------------------------------------------------------------------------
    qreal  inv_rg_sz = 1.0 / ( vw_port.isValid() ? vw_port.width() : ln_par.max() - ln_par.min() );
    qreal  bd_ofv = bounds.x(), bd_sz = bounds.width();
    float  px_pos = float( bounds.height() * 0.5 );

    // pre-alloc point in geometry buffer
    if ( tick_info.hasTickInVrg()) {
        int  tick_sz = int( tick_info.nrMaxTickIdxInt() - tick_info.nrMinTickIdxInt() + 1 );
        m_geom.allocate( ( ln_par.isShowBln() ? 4 : 0 ) + 4 * tick_sz );
    } else {
        m_geom.allocate( ln_par.isShowBln() ? 4 : 0 );
    }
    if ( ! tick_info.hasTickInVrg()  &&  ! ln_par.isShowBln()) { return; }

    IcQSGValueAxisNodeSpc::LnVertex *vtx = static_cast< IcQSGValueAxisNodeSpc::LnVertex *>( m_geom.vertexData() );
    float px_fix = ( float(0.0005) + mater.size ) * float(0.5);
    int vtx_ofv  = 0;

    // generate the horizontal base line points
    if ( ln_par.isShowBln() ) {
        qreal rg_ofv = ( vw_port.isValid() ? vw_port.x() : ln_par.blnMin() );
        qreal pct_pos = ( tick_info.brgMin() - rg_ofv ) * inv_rg_sz;

        qreal sc_pos  = pct_pos * bd_sz + bd_ofv;
        vtx[0].set( float( sc_pos ), px_pos - px_fix, 0 );
        vtx[1].set( float( sc_pos ), px_pos + px_fix, 1 );

        pct_pos = ( tick_info.brgMax() - rg_ofv ) * inv_rg_sz;
        sc_pos  = pct_pos * bd_sz + bd_ofv;
        vtx[2].set( float( sc_pos ), px_pos + px_fix, 1 );
        vtx[3].set( float( sc_pos ), px_pos - px_fix, 0 );

        vtx_ofv = 4;
    }

    // generate the points for ticks
    if ( tick_info.hasTickInVrg()) {
        qint64 nr_min_idx = tick_info.nrMinTickIdxInt(); // nearest minimum tick index
        qint64 nr_max_idx = tick_info.nrMaxTickIdxInt(); // nearest maxmum  tick index
        qreal rg_ofv = ( vw_port.isValid() ? vw_port.x() : ln_par.min() ); //

        qint64 mj_f_rd_v  = ln_par.minorTickCnt() + 1;
        qint64 mn_hf_rd_v = ( ln_par.minorHintStep() > 0 ? ln_par.minorHintStep() : mj_f_rd_v );
        qint64 mj_f_cntr  = nr_min_idx % mj_f_rd_v;
        qint64 mn_hf_cntr = nr_min_idx % mn_hf_rd_v;
        if ( mj_f_cntr  == 0 ) { mj_f_cntr  = mj_f_rd_v; }
        if ( mn_hf_cntr == 0 ) { mn_hf_cntr = mn_hf_rd_v; }

        // setup the label layout data
        if ( lbl_layout != nullptr ) {
            lbl_layout->init (
                nr_min_idx, nr_max_idx, rg_ofv, 1.0 / inv_rg_sz, tick_info.baseTickStep(), ln_par.min(),
                mj_f_rd_v, mn_hf_rd_v
            );
        }

        // a direction trick for later use ( reduce the 'if' sentence )
        float  tk_ccw_sz = 0, tk_cw_sz = 0;
        float *tk_ccw_ptr = &tk_ccw_sz, *tk_cw_ptr = &tk_cw_sz;
        switch( ln_par.tickDir() ) {
        case IcQSGValueAxisNode::LnParam::TickDir_CCW : tk_cw_ptr  = tk_ccw_ptr; break;
        case IcQSGValueAxisNode::LnParam::TickDir_CW  : tk_ccw_ptr = tk_cw_ptr;  break;
        default: break;
        }

        for ( qint64 i = nr_min_idx; i <= nr_max_idx; i ++, mj_f_cntr ++, mn_hf_cntr ++ ) {
           // overwite the tick size by ccw and cw, here used a pointer trick tech.
           // Assume CCW and CW ticks are used.
           *tk_ccw_ptr = float( ln_par.minorSize()); *tk_cw_ptr = *tk_ccw_ptr;
           if ( mn_hf_cntr == mn_hf_rd_v ) {
              *tk_ccw_ptr = float( ln_par.minorStepSize()); *tk_cw_ptr = *tk_ccw_ptr;
              mn_hf_cntr = 0;
           }
           if ( mj_f_cntr == mj_f_rd_v ) {
              *tk_ccw_ptr = float( ln_par.majorSize()); *tk_cw_ptr = *tk_ccw_ptr;
              mj_f_cntr = 0;
           }

           // translate the math point to item coord.
           qreal pct_pos = ( tick_info.baseTickStep() * qreal(i) + ln_par.min() - rg_ofv ) * inv_rg_sz;
           qreal sc_pos  = pct_pos * bd_sz + bd_ofv;

           // the GL draw coord is window coord, not the math coord. we need flip the y coord here.
           // the cw and ccw is flipped.
           if ( pct_pos >= 0 && pct_pos <= 1.0 ) {
               vtx[ vtx_ofv     ].set( float( sc_pos ) + px_fix, px_pos - tk_ccw_sz, 0 );
               vtx[ vtx_ofv + 1 ].set( float( sc_pos ) - px_fix, px_pos - tk_ccw_sz, 1 );
               vtx[ vtx_ofv + 2 ].set( float( sc_pos ) - px_fix, px_pos + tk_cw_sz,  1 );
               vtx[ vtx_ofv + 3 ].set( float( sc_pos ) + px_fix, px_pos + tk_cw_sz,  0 );
           } else {
               // while less than 0, drop this tick ( make it draw on zero point ... )
               vtx[ vtx_ofv ].set( 0, 0, 0 );
               vtx[ vtx_ofv + 1 ].set( 0, 0, 1 );
               vtx[ vtx_ofv + 2 ].set( 0, 0, 1 );
               vtx[ vtx_ofv + 3 ].set( 0, 0, 0 );
           }
           vtx_ofv += 4;

       }
    } else {
        if ( lbl_layout != nullptr ) { lbl_layout->invalidate(); }
    }

    markDirty( QSGNode::DirtyGeometry );
}



}
#endif
