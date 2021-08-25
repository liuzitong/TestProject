�� ScMemBlock ��

������ԣ�
   1) �������ڲ�ͬ C++ ���п���д��ݣ����ã�ָ�룬���Ƶȵȷ�ʽ������֤����ģ�����Ӳ�ͬ���п�ʱ��ȫ��

   2����֤ǰ��汾�����ԡ�
      �� ScMemBlock 0.1.0 ���ڴ������� ScMemBlock 0.2.0 �����汾��ȫ�ͷ�, ��֮��ɡ�

   3) ʵ����Ϊ���������ѡд��
      ScMemBlock ���츴�ƣ���ֵ���Ʋ�ȡ��ʽ����ʽ��ʹ���߿��Լ򵥵ؽ��������һ�����ݵ���Ҫʹ����
      ����Զ�����С�

   4) ֧���ֶ���ʽ�Ĺ����ơ�

   5) ֧�ַ�������ָ��ʱ�жϸ��ơ�

   6) ֧�ֿ��̴߳��ݡ�
      ���棺����֤�̶߳�д��ȫ�ԣ�����֤�������ȫ���߳�֮�䴫��ʱ������ֵ��ʽ���ݣ�������ָ�뷽ʽ���ݣ�

   *) ���õ����ڴ��ͷţ����ڲ����ü�������ʱ���Զ��ͷš�
      �ڴ��β��׷��У���ֽڣ��ɼ�⵽д����ʱ�����߽��һЩ�����

API ˵����

    ScMemBlock()
    ����: ����һ���ڴ������������������ָ��յ�ַ ( nullptr )

    ScMemBlock( size_t sz )
    ����������һ��ָ����С���ڴ��������
    ������sz -- �ֽڳߴ磬��� < 1, ���ͬ�� ScMemBlock() 
    
    ScMemBlock( size_t sz, bool ( *UserProc )( ActID act_id, void *act_param, void* ), void* )
    �汾��0.2.0 ����
    ����������һ��ָ����С���ڴ�飬���ҹҽӻص��������Զ��崦���ڴ��Ļ
    ������sz -- �ֽڳߴ磬��� < 1�����ͬ�� ScMemBlock()
          UserProc -- �ص����������Ϊ nullptr, ��ʹ��Ĭ�ϴ�������
    ��أ�
          ActID ��Ϊö��ֵ
          ActID_Init   : ���ڴ��������  ������0 �Ȳ�����ʱ�ص���act_param ���� struct ActInitParam �ṹָ��
          ActID_Destroy: ���ڴ����ͷ�ǰ����, act_param ���� struct ActDestroyParam �ṹָ��
          ActID_InitByCopy : ���ڴ��������ʱ���ã�������Ӧ�ø���ִ�����ݴ�һ�����Ƶ���һ������Ϊ��act_param ���� struct ActInitByCopyParam ָ��
          
          struct ActInitParam {   
              void    *data;    // ����ָ��
              sizez_t  data_sz; // �����ֽ���
          };

          struct ActDestroyParam {
              void    *data;    // ����ָ��
              size_t   data_sz; // �����ֽ���
          };          

          struct ActInitByCopyParam {
              void   *tgt_data; // ���Ƶ���Ŀ���ַ
              size_t  tgt_sz;   // Ŀ���ַ��Ч�ֽ���
              void   *src_data; // ���Ƶ���Դ��ַ
              size_t  src_sz;   // ��Դ��ַ����Ч�ֽ���
          };

          * ע�⣺tgt_sz > src_sz ʱ��ע�� tgt_data û�����ݿɸ��ƵĲ����Ƿ���Ҫ��ʼ����

    ScMemBlock( const ScMemBlock &ot )
    ���������ƹ���ָ�����ڴ�����������ǰ����������ָ���������������ڴ档
    ������ot -- ָ�����ڴ�������
    ע�⣺ִ�д˲����󣬵�ǰ�������� 'ot' ����ͬһƬ�ڴ档

    ScMemBlock &  operator = ( const ScMemBlock &ot )
    ����������������ڴ�飨����У������ӵ�ָ������������ָ����ڴ档
    ������ot -- ָ�����ڴ�������
    ע�⣺ִ�д˲����󣬵�ǰ�������� 'ot' ����ͬһƬ�ڴ档

    void  makeNull()
    �汾��0.1.0 ����
    ������ǿ���ͷŸ�����ָ����ڴ棬�ø�������ָ��յ�ַ
    ע�⣺������������������������������ڴ棬��˲���ֻ�ǶϿ�������ڴ档
    ��أ��˲�����ͬ�����µ��ã�
          ScMemBlock mb(100); mb = ScMemBlock();
    
    bool  isNull() const
    �������жϵ�ǰ�������Ƿ���Ч����Чָ�ڴ�������ָ��յ�ַ
    ���أ�true ����ǰ��������Ч��ָ����Ч���ڴ��ַ��

    size_t  size() const
    ���أ���ǰ��������ָ����ڴ��С
    ע�⣺����ڴ���������Ч��isNull() ���� true��size() ���� 0

    void    resize( size_t sz )
    �汾��0.2.0 ����
    ���������±���ڴ��������ڴ��С
    ������sz -- �����Ĵ�С
    ע�⣺�����ǰ�ڴ�������Ϊ��Ч, ����ô˺�����, ��ӵ��ָ����С���ڴ�顣
          �����ǰ�ڴ��������������ڴ�����������ͬһƬ�ڴ棬��ǰ�ڴ����������·��� sz ��С�ռ䣬
          ͬʱ����ԭ���ڴ��������е����ݡ���������ı乲������ݣ�
          
    const void*  constData() const
    ������ֱ�ӷ����ڴ��ַ�������ǰ������Ϊ��Ч���򷵻� nullptr
    ���أ��ڴ��ַ
    ���棺�����ǰ�����������������ڴ棬���޸��ڴ����ݽ�Ӱ��������������
    
    ScMemBlock  copy() const
    ���أ����Ƶ�ǰ���ڴ棬���ظ��ڴ��������
    ע�⣺�����ǰ�ڴ���������Ч���򷵻���Ч���ڴ���������

    bool        dupIfShared()
    �����������ǰ�������������������������ڴ棬��ִ�и��Ʋ������õ�ǰ�ڴ���������Ϊ�����ڴ�������
    ���أ�true  -- �����˸��ƣ�  ��ǰ�������Ѹ���һ���ڴ�
          false -- û�з������ƣ���ǰ��������Ϊ����

    void*       dataCow() 
    �汾��0.2.0 ����
    ����������һ����д������ָ�룬���ұ�֤����Ϊ��������Ϊ����ʽʱ����ȡ dupIfShared() ��ͬ�ķ�ʽ���и��ơ�
    ���أ���д������ָ��
    ע�⣺�����˽⵽��ִ�д˺����󣬿��ܻ��������ݸ��ƣ�   

    void        swap( ScMemBlock &ot )
    �汾��0.3.0 ����
    ��������ָд�Ķ��󽻻�����    

ʹ��ʾ����
   [ �����յ��ڴ�� ]
       QxPack::ScMemBlock mb();
       bool is_valid = mb.isNull();    // is_valid Ϊ true

   [ ����ָ����С���ڴ�� ]
       QxPack::ScMemBlock mb(100);     // ������100���ֽڵĿռ�
        
   [ ���ӵ������ڴ���ϣ�����������Ŀռ� ]
       QxPack::ScMemBlock mb(100);
       QxPack::ScMemBlock mb2( mb );   // ͨ�����ƹ��췽ʽ���ӵ������ڴ��
       QxPack::ScMemBlock mb3 = mb;    // ͨ����ֵ��ʽ���ӵ������ڴ��

   [ ǿ�Ʒ����ڴ� ]
       QxPack::ScMemBlock mb(100);    
       mb.makeNull();                  // ǿ�Ʒ����ڴ�
       bool is_valid = mb.isNull();    // is_valid Ϊ true

   [ ��ʽ����ָ�����ڴ�� ]
       QxPack::ScMemBlock mb(100);
       QxPack::ScMemBLock mb2 = mb.copy(); // ��ʽ���ƣ�mb2 ָ��Ŀռ��� mb �Ŀռ��ǲ�һ���ġ�
        
   [ �жϹ���ʱ�Ÿ���  ]
       QxPack::ScMemBlock mb(100);
       bool is_dup = mb.dupIfShared();     // is_dup Ϊ false, ʲôҲû�з�������Ϊ mb û���������鹲��ռ�

       QxPack::ScMemBlock mb2 = mb;        // mb2 �� mb ָ��ͬһƬ�ռ�
       QxPack::ScMemBlock mb3 = mb;        // mb,mb2 ָ��ͬһƬ�ռ�
       mb3.dupIfShared();                  // ִ�д˾䣬 mb3 ָ��ͬ�Ŀռ䡣

   [ ȡֻ������ָ�� ]
       QxPack::ScMemBlock mb(100);
       const void* cptr = mb.constData();  // cptr �������ݿռ��ַ
        
   [ ȡ��д����ָ�룬�����������¸������ݺ���ȡָ�� ]
       QxPack::ScMemBlock mb(100);
       void *dp  = mb.dataCow();           // mb û�з������ƣ���Ϊ mb ָ��Ŀռ�ֻ��һ������
       
       QxPack::ScMemBlock mb2 = mb;
       void *dp2 = mb.dataCow();           // mb �����˸��ƣ���Ϊ mb2 �� mb ��ָͬ��ͬһƬ�ռ�

   [ ��Ϊ������������� ]
       
       // ���º�����ʾֻ���������޸ĸ��ƣ������ڴ��� 
       QxPack::ScMemBlock  foo( const QxPack::ScMemBlock &mb ) 
       {
           printf("mem block size:%u\n", mb.size());
        
           QxPack::ScMemBlock mb2 = mb; // �ȸ��ӵ����ڴ�飬�ɹ����

           const uint8_t *cdp = reinterpret_cast<const uint8_t*>( mb2.constData());
           .....
           
           uint8_t *dp = reinterpret_cast<uint8_t*>( mb2.dataCow()); // ����һ��
           dp[0] = 0xaa; // �޸�����
           
           return mb2;   // �����޸Ĺ��Ľ�� 
       }       

       int main( int argc, void** argv ) 
       {
           QxPack::ScMemBlock mb( 100 );

           QxPack::ScMemBlock mb2 = foo( mb ); // ȡ�ý���ڴ��

           return 0;
       }

   
   
