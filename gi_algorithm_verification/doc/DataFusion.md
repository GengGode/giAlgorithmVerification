# INS & GPS Data Fusion ���Ե��������Ƕ�λ�������ں�

## 1. ��������

### 1.1 ���ⱳ��

�����˻��ĵ���ϵͳ�У����Ե���ϵͳ��INS�������Ƕ�λϵͳ��GPS�������ֳ����ĵ���ϵͳ��INS��һ�ֻ��ڹ��Բ�����Ԫ��IMU���ĵ���ϵͳ���������ṩ�߾��ȵ�λ�á��ٶȺ���̬��Ϣ��GPS��һ�ֻ������ǵĵ���ϵͳ���������ṩ�߾��ȵ�λ����Ϣ�������˻��ĵ���ϵͳ�У�INS��GPS�������ںϿ����ṩ���߾��ȵ�λ�á��ٶȺ���̬��Ϣ��

### 1.2 ��������

���ֵ�����λ������ȱ������

INS���ŵ��ǣ�
- 1�������ṩ��Ƶ�ʵ����ݣ�
- 2�������ṩ�߿ɿ��Ե����ݡ�

GPS���ŵ��ǣ�
- 1�������ṩ�߾��ȵ�λ����Ϣ��

���ֵ���ϵͳ�������ںϿ����ṩ���߾��ȵ�λ�á��ٶȺ���̬��Ϣ��

## 2. �����ں��㷨

### 2.1 �����ں��㷨�Ļ���˼��

�����ں��㷨�Ļ���˼���ǣ�ͨ����INS��GPS�����ݽ����ںϣ������ṩ���߾��ȵ�λ�á��ٶȺ���̬��Ϣ��

### 2.2 �����ں��㷨�Ļ�������

�����ں��㷨�Ļ�����������ͼ��ʾ��

![�����ں��㷨�Ļ�������](./images/ins_gps_fusion.png)

## 3. �����ں��㷨��ʵ��

### 3.1 �����ں��㷨��ʵ������

�����ں��㷨��ʵ����������ͼ��ʾ��

![�����ں��㷨��ʵ������](./images/ins_gps_fusion_implementation.png)

### 3.2 �����ں��㷨��ʵ�ִ���

�����ں��㷨��ʵ�ִ���������ʾ��

```cpp
#include "data_fusion.h"

namespace gi_algorithm_verification {

DataFusion::DataFusion() {
  // TODO
}

DataFusion::~DataFusion() {
  // TODO
}

void DataFusion::Init() {
  // TODO
}

void DataFusion::Run() {
  // TODO
}

}  // namespace gi_algorithm_verification
```

## 4. �����ں��㷨�ĵ�Ԫ����

### 4.1 �����ں��㷨�ĵ�Ԫ��������

�����ں��㷨�ĵ�Ԫ������������ͼ��ʾ��

![�����ں��㷨�ĵ�Ԫ��������](./images/ins_gps_fusion_unit_test.png)

### 4.2 �����ں��㷨�ĵ�Ԫ���Դ���

�����ں��㷨�ĵ�Ԫ���Դ���������ʾ��

```cpp

#include "data_fusion.h"

#include "gtest/gtest.h"

namespace gi_algorithm_verification {

TEST(DataFusion, Run) {
  DataFusion data_fusion;
  data_fusion.Init();
  data_fusion.Run();
}

}  // namespace gi_algorithm_verification

```

## 5. �����ں��㷨�ļ��ɲ���

### 5.1 �����ں��㷨�ļ��ɲ�������

�����ں��㷨�ļ��ɲ�����������ͼ��ʾ��

![�����ں��㷨�ļ��ɲ�������](./images/ins_gps_fusion_integration_test.png)

### 5.2 �����ں��㷨�ļ��ɲ��Դ���

�����ں��㷨�ļ��ɲ��Դ���������ʾ��

```cpp

#include "data_fusion.h"

#include "gtest/gtest.h"

namespace gi_algorithm_verification {

TEST(DataFusion, Run) {
  DataFusion data_fusion;
  data_fusion.Init();
  data_fusion.Run();
}

}  // namespace gi_algorithm_verification

```

## 6. �����ں��㷨�����ܲ���