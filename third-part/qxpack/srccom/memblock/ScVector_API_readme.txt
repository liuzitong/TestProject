�� ScVector ��

������ԣ�
    1) ���� ScMemBlock ��ƣ�֧��ȫ�� ScMemBlock ������

    2) ʹ��ģ�巽ʽ��ƣ����֧�ֻ������� ( fix_step < 2 ), �Լ��Զ������� ( fix_step >= 2 )
       �Զ�����������Ҫʵ�� ���ƹ��� ���� ��ֵ����
   
    3) ʹ��[] �±���з���ʱ����Ҫ�ر�ע��C++ ��const ���ԡ�
       3-1) Ĭ������£�[] ������  T &  operator[]( int i ) ��ʵ�֣���ʵ�ֽ�ÿ�μ��
            �Ƿ�����˹���������ѹ������ݣ���ִ�и��ƺ����������ݣ�����ͼ����ʹ�����޸ĳ�Ա���ݵ���ͼ��

       3-2) ����ʹ������ͼΪֻ��ȡ���ݣ���������ʽʹ�ý�������Ƿ���
            const ScVector<T> &vec_const = vec;  // vec Ϊ���ݼ�
            T & val = vec_const[ i ];            // ��ʱ������  const T& operator[]( int ) const ��ʵ��
            const T &val2 = vec_const[i];    

       * Ϊȷ��ֻ�����ʣ���ʹ�� ScVector::at( size_t idx ) ����       
       
API ˵����     
     ScVector<T>()
     ����������һ���յĶ���

     ScVector<T>( size_t dim )
     ����������һ��ָ����С�ռ�Ķ���
     ������dim [IN] -- һά��С������Ҫ�Ŀռ��С
     
     ScVector<T>( const ScVector<T> &ot )
     ���������ƹ���ָ���Ķ��󡣵�ǰ����������ָ��������ͬ�������� 
     ������ot [IN] -- ָ���Ķ���
     ע�⣺ִ�д˲����󣬵�ǰ������ 'ot' ������ͬ������

     ScVector<T>& operator = ( const ScVector<T> &ot )
     �������������������ݣ�����У������ӵ�ָ���Ķ�������ݡ�
     ������ot [IN] -- ָ���Ķ���
     ע�⣺ִ�д˲����󣬵�ǰ���������� 'ot' ����ͬһƬ���ݡ�

     bool     isNull() const
     �������жϵ�ǰ�����Ƿ���Ч��
     ���أ�true ����������Ч��

     size_t   dim() const
     ���������ع����������ʱ�����һά��С��ֵ
     
     const T* constData()
     ����������ֻ���������׵�ַ
     ���أ�ֻ��ָ�룬�����ǰ������Ч���򷵻� nullptr
     ���棺����޸����ݣ������Ӱ�������������ݵĶ���
     
     bool     dupIfShared()
     �������жϵ�ǰ�����Ƿ�������������һƬ���ݿռ䣬����ǣ��������ݣ��������κ���
     ���أ�true ���������ݣ�false û�и���
     ע�⣺���ʵ������T Ϊ�Զ��������࣬����ܲ������������ݹ��츴�ƵȲ�����

     T*       dataCow()     
     �������жϵ�ǰ�����Ƿ�������������һƬ���ݿռ䣬����ǣ��������ݣ�ͬʱ���ؿ�д����ָ��
     ���أ�����ָ�롣�������Ϊ��Ч״̬���򷵻� nullptr

     ScVector<T> copy() const
     ������ֱ�Ӹ���һ�����ݲ�����
     ע�⣺�����ǰ������Ч��Ҳ������Ч�Ķ���

     template <typename Ti>
     const T&  operator[]( const Ti &i ) const
     ���������±���ʷ�ʽ����ָ���������������õ�ַ
     ע�⣺�±겻���Գ�������������� dim()��
     �����fix_step < 2 �˺����Ƿ�ģ�庯������ fix_step > 2 ���Ϊģ�庯�� 
    
     template <typename Ti>
     T&  operator[]( const Ti &i )
     �±�Ŀ��޸����ݰ汾��
     
     void    makeNull()
     �汾��fix_step == 2 ����
     ������ǿ���ͷ�����
     ע�⣺��������������������������������ݣ���˲���ֻ�ǶϿ����������ݡ�

     size_t  size() const
     �汾��fix_step == 2 ����
     ���������ص�ǰ����Ч���ݸ���
     ע�⣺�� fix_step < 2 ʱ����ֵ�� dim() ��ͬ��fix_step >= 2 ʱ������Ϊ�Ժ�֧�ֶ�̬Ԫ�ص�һ��
     ���������ڷ��ص�ǰ��Ч��Ԫ�ظ������Ժ����ӵ� push_back ��

     size_t  capacity() const
     �汾��fix_step == 2 ����
     ���������������������

     void   push_back( const T& v )
     �汾��fix_step == 3 ����
     ��������������ѹ��һ��Ԫ�أ��������̫С����Ϊ�գ���ռ��Զ�����������Ҫ��

     void   pop_back()
     �汾��fix_step == 3 ����
     ������������β��ɾ��һ��Ԫ��

     void   push_front( const T& v )
     �汾��fix_step == 3 ����
     ����������ǰ��ѹ��һ��Ԫ��
     ���棺��ǰ��������Ԫ�ؾ�������һ����λ�����������������ƶ���
     ע�⣺�������Ϊ class�������ʵ�� �ƶ���ֵ ������ʵ�ֽϵ͵������ƶ��ɱ���    

     void   pop_front()
     �汾��fix_step == 3 ����
     ����������ǰ��ɾ��һ��Ԫ��          
     
     bool   empty() const
     �汾��fix_step == 3 ����
     �������ж������Ƿ�Ϊ��
     ���أ�true Ϊ��

     size_t  max_size() const
     �汾��fix_step == 3 ����
     ���������ݣ�ֻ�Ƿ���һ�����ֵ

     void    clear()
     �汾��fix_step == 3 ����
     ��������������е���Ŀ

     void    reserve( size_t n )
     �汾��fix_step == 3 ����
     ������Ԥ�����ռ䣬�Ա���ִ�г�Ա����ʱ�ٶ�̬�����ڴ�

     void    resize( size_t n )
     �汾��fix_step == 3 ����
     ������������չԪ�ص� n�����������Ԫ������ n����Ĭ�ϲ���Ԫ�ء�
           ���Ԫ����Ŀ���� n, ���ͷŶ����Ԫ�� 
     
     void    insert( size_t pos, const T&v )
     �汾��fix_step == 3 ����
     ��������ָ��λ�ò���Ԫ�ء��磺1,3,5  ���±� 1 �ϲ���9 ����� 1,9,3,5
      
     void    remove( size_t pos )
     �汾��fix_step == 3 ����
     �������Ƴ�ָ��λ���ϵ�Ԫ�ء��磺1,3,5 �Ƴ��±� 1 ��Ԫ�أ������ 1,5

     const T&   back() const
     �汾��fix_step == 3 ����
     ����������������ĩβ��ֻ��Ԫ��

     const T&   front() const
     �汾��fix_step == 3 ����
     ����������������ǰ����ֻ��Ԫ��

     const T&   at( size_t pos )
     �汾��fix_step == 3 ����
     ����������ָ���±��Ԫ��ֻ������
     
    
     