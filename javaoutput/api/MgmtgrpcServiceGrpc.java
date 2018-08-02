package api;

import static io.grpc.MethodDescriptor.generateFullMethodName;
import static io.grpc.stub.ClientCalls.asyncBidiStreamingCall;
import static io.grpc.stub.ClientCalls.asyncClientStreamingCall;
import static io.grpc.stub.ClientCalls.asyncServerStreamingCall;
import static io.grpc.stub.ClientCalls.asyncUnaryCall;
import static io.grpc.stub.ClientCalls.blockingServerStreamingCall;
import static io.grpc.stub.ClientCalls.blockingUnaryCall;
import static io.grpc.stub.ClientCalls.futureUnaryCall;
import static io.grpc.stub.ServerCalls.asyncBidiStreamingCall;
import static io.grpc.stub.ServerCalls.asyncClientStreamingCall;
import static io.grpc.stub.ServerCalls.asyncServerStreamingCall;
import static io.grpc.stub.ServerCalls.asyncUnaryCall;
import static io.grpc.stub.ServerCalls.asyncUnimplementedStreamingCall;
import static io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall;

/**
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.14.0)",
    comments = "Source: api.proto")
public final class MgmtgrpcServiceGrpc {

  private MgmtgrpcServiceGrpc() {}

  public static final String SERVICE_NAME = "api.MgmtgrpcService";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<api.Api.CreateRequest,
      api.Api.CreateReply> getCreateSapphireObjectMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "CreateSapphireObject",
      requestType = api.Api.CreateRequest.class,
      responseType = api.Api.CreateReply.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<api.Api.CreateRequest,
      api.Api.CreateReply> getCreateSapphireObjectMethod() {
    io.grpc.MethodDescriptor<api.Api.CreateRequest, api.Api.CreateReply> getCreateSapphireObjectMethod;
    if ((getCreateSapphireObjectMethod = MgmtgrpcServiceGrpc.getCreateSapphireObjectMethod) == null) {
      synchronized (MgmtgrpcServiceGrpc.class) {
        if ((getCreateSapphireObjectMethod = MgmtgrpcServiceGrpc.getCreateSapphireObjectMethod) == null) {
          MgmtgrpcServiceGrpc.getCreateSapphireObjectMethod = getCreateSapphireObjectMethod = 
              io.grpc.MethodDescriptor.<api.Api.CreateRequest, api.Api.CreateReply>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(
                  "api.MgmtgrpcService", "CreateSapphireObject"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  api.Api.CreateRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  api.Api.CreateReply.getDefaultInstance()))
                  .setSchemaDescriptor(new MgmtgrpcServiceMethodDescriptorSupplier("CreateSapphireObject"))
                  .build();
          }
        }
     }
     return getCreateSapphireObjectMethod;
  }

  private static volatile io.grpc.MethodDescriptor<api.Api.DeleteRequest,
      api.Api.DeleteReply> getDeleteSapphireObjectMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DeleteSapphireObject",
      requestType = api.Api.DeleteRequest.class,
      responseType = api.Api.DeleteReply.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<api.Api.DeleteRequest,
      api.Api.DeleteReply> getDeleteSapphireObjectMethod() {
    io.grpc.MethodDescriptor<api.Api.DeleteRequest, api.Api.DeleteReply> getDeleteSapphireObjectMethod;
    if ((getDeleteSapphireObjectMethod = MgmtgrpcServiceGrpc.getDeleteSapphireObjectMethod) == null) {
      synchronized (MgmtgrpcServiceGrpc.class) {
        if ((getDeleteSapphireObjectMethod = MgmtgrpcServiceGrpc.getDeleteSapphireObjectMethod) == null) {
          MgmtgrpcServiceGrpc.getDeleteSapphireObjectMethod = getDeleteSapphireObjectMethod = 
              io.grpc.MethodDescriptor.<api.Api.DeleteRequest, api.Api.DeleteReply>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(
                  "api.MgmtgrpcService", "DeleteSapphireObject"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  api.Api.DeleteRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  api.Api.DeleteReply.getDefaultInstance()))
                  .setSchemaDescriptor(new MgmtgrpcServiceMethodDescriptorSupplier("DeleteSapphireObject"))
                  .build();
          }
        }
     }
     return getDeleteSapphireObjectMethod;
  }

  private static volatile io.grpc.MethodDescriptor<api.Api.GenericMethodRequest,
      api.Api.GenericMethodReply> getGenericMethodInvokeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "GenericMethodInvoke",
      requestType = api.Api.GenericMethodRequest.class,
      responseType = api.Api.GenericMethodReply.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<api.Api.GenericMethodRequest,
      api.Api.GenericMethodReply> getGenericMethodInvokeMethod() {
    io.grpc.MethodDescriptor<api.Api.GenericMethodRequest, api.Api.GenericMethodReply> getGenericMethodInvokeMethod;
    if ((getGenericMethodInvokeMethod = MgmtgrpcServiceGrpc.getGenericMethodInvokeMethod) == null) {
      synchronized (MgmtgrpcServiceGrpc.class) {
        if ((getGenericMethodInvokeMethod = MgmtgrpcServiceGrpc.getGenericMethodInvokeMethod) == null) {
          MgmtgrpcServiceGrpc.getGenericMethodInvokeMethod = getGenericMethodInvokeMethod = 
              io.grpc.MethodDescriptor.<api.Api.GenericMethodRequest, api.Api.GenericMethodReply>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(
                  "api.MgmtgrpcService", "GenericMethodInvoke"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  api.Api.GenericMethodRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  api.Api.GenericMethodReply.getDefaultInstance()))
                  .setSchemaDescriptor(new MgmtgrpcServiceMethodDescriptorSupplier("GenericMethodInvoke"))
                  .build();
          }
        }
     }
     return getGenericMethodInvokeMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static MgmtgrpcServiceStub newStub(io.grpc.Channel channel) {
    return new MgmtgrpcServiceStub(channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static MgmtgrpcServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    return new MgmtgrpcServiceBlockingStub(channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static MgmtgrpcServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    return new MgmtgrpcServiceFutureStub(channel);
  }

  /**
   */
  public static abstract class MgmtgrpcServiceImplBase implements io.grpc.BindableService {

    /**
     */
    public void createSapphireObject(api.Api.CreateRequest request,
        io.grpc.stub.StreamObserver<api.Api.CreateReply> responseObserver) {
      asyncUnimplementedUnaryCall(getCreateSapphireObjectMethod(), responseObserver);
    }

    /**
     */
    public void deleteSapphireObject(api.Api.DeleteRequest request,
        io.grpc.stub.StreamObserver<api.Api.DeleteReply> responseObserver) {
      asyncUnimplementedUnaryCall(getDeleteSapphireObjectMethod(), responseObserver);
    }

    /**
     */
    public void genericMethodInvoke(api.Api.GenericMethodRequest request,
        io.grpc.stub.StreamObserver<api.Api.GenericMethodReply> responseObserver) {
      asyncUnimplementedUnaryCall(getGenericMethodInvokeMethod(), responseObserver);
    }

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            getCreateSapphireObjectMethod(),
            asyncUnaryCall(
              new MethodHandlers<
                api.Api.CreateRequest,
                api.Api.CreateReply>(
                  this, METHODID_CREATE_SAPPHIRE_OBJECT)))
          .addMethod(
            getDeleteSapphireObjectMethod(),
            asyncUnaryCall(
              new MethodHandlers<
                api.Api.DeleteRequest,
                api.Api.DeleteReply>(
                  this, METHODID_DELETE_SAPPHIRE_OBJECT)))
          .addMethod(
            getGenericMethodInvokeMethod(),
            asyncUnaryCall(
              new MethodHandlers<
                api.Api.GenericMethodRequest,
                api.Api.GenericMethodReply>(
                  this, METHODID_GENERIC_METHOD_INVOKE)))
          .build();
    }
  }

  /**
   */
  public static final class MgmtgrpcServiceStub extends io.grpc.stub.AbstractStub<MgmtgrpcServiceStub> {
    private MgmtgrpcServiceStub(io.grpc.Channel channel) {
      super(channel);
    }

    private MgmtgrpcServiceStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected MgmtgrpcServiceStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new MgmtgrpcServiceStub(channel, callOptions);
    }

    /**
     */
    public void createSapphireObject(api.Api.CreateRequest request,
        io.grpc.stub.StreamObserver<api.Api.CreateReply> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(getCreateSapphireObjectMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void deleteSapphireObject(api.Api.DeleteRequest request,
        io.grpc.stub.StreamObserver<api.Api.DeleteReply> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(getDeleteSapphireObjectMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void genericMethodInvoke(api.Api.GenericMethodRequest request,
        io.grpc.stub.StreamObserver<api.Api.GenericMethodReply> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(getGenericMethodInvokeMethod(), getCallOptions()), request, responseObserver);
    }
  }

  /**
   */
  public static final class MgmtgrpcServiceBlockingStub extends io.grpc.stub.AbstractStub<MgmtgrpcServiceBlockingStub> {
    private MgmtgrpcServiceBlockingStub(io.grpc.Channel channel) {
      super(channel);
    }

    private MgmtgrpcServiceBlockingStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected MgmtgrpcServiceBlockingStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new MgmtgrpcServiceBlockingStub(channel, callOptions);
    }

    /**
     */
    public api.Api.CreateReply createSapphireObject(api.Api.CreateRequest request) {
      return blockingUnaryCall(
          getChannel(), getCreateSapphireObjectMethod(), getCallOptions(), request);
    }

    /**
     */
    public api.Api.DeleteReply deleteSapphireObject(api.Api.DeleteRequest request) {
      return blockingUnaryCall(
          getChannel(), getDeleteSapphireObjectMethod(), getCallOptions(), request);
    }

    /**
     */
    public api.Api.GenericMethodReply genericMethodInvoke(api.Api.GenericMethodRequest request) {
      return blockingUnaryCall(
          getChannel(), getGenericMethodInvokeMethod(), getCallOptions(), request);
    }
  }

  /**
   */
  public static final class MgmtgrpcServiceFutureStub extends io.grpc.stub.AbstractStub<MgmtgrpcServiceFutureStub> {
    private MgmtgrpcServiceFutureStub(io.grpc.Channel channel) {
      super(channel);
    }

    private MgmtgrpcServiceFutureStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected MgmtgrpcServiceFutureStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new MgmtgrpcServiceFutureStub(channel, callOptions);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<api.Api.CreateReply> createSapphireObject(
        api.Api.CreateRequest request) {
      return futureUnaryCall(
          getChannel().newCall(getCreateSapphireObjectMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<api.Api.DeleteReply> deleteSapphireObject(
        api.Api.DeleteRequest request) {
      return futureUnaryCall(
          getChannel().newCall(getDeleteSapphireObjectMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<api.Api.GenericMethodReply> genericMethodInvoke(
        api.Api.GenericMethodRequest request) {
      return futureUnaryCall(
          getChannel().newCall(getGenericMethodInvokeMethod(), getCallOptions()), request);
    }
  }

  private static final int METHODID_CREATE_SAPPHIRE_OBJECT = 0;
  private static final int METHODID_DELETE_SAPPHIRE_OBJECT = 1;
  private static final int METHODID_GENERIC_METHOD_INVOKE = 2;

  private static final class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final MgmtgrpcServiceImplBase serviceImpl;
    private final int methodId;

    MethodHandlers(MgmtgrpcServiceImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_CREATE_SAPPHIRE_OBJECT:
          serviceImpl.createSapphireObject((api.Api.CreateRequest) request,
              (io.grpc.stub.StreamObserver<api.Api.CreateReply>) responseObserver);
          break;
        case METHODID_DELETE_SAPPHIRE_OBJECT:
          serviceImpl.deleteSapphireObject((api.Api.DeleteRequest) request,
              (io.grpc.stub.StreamObserver<api.Api.DeleteReply>) responseObserver);
          break;
        case METHODID_GENERIC_METHOD_INVOKE:
          serviceImpl.genericMethodInvoke((api.Api.GenericMethodRequest) request,
              (io.grpc.stub.StreamObserver<api.Api.GenericMethodReply>) responseObserver);
          break;
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        default:
          throw new AssertionError();
      }
    }
  }

  private static abstract class MgmtgrpcServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    MgmtgrpcServiceBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return api.Api.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("MgmtgrpcService");
    }
  }

  private static final class MgmtgrpcServiceFileDescriptorSupplier
      extends MgmtgrpcServiceBaseDescriptorSupplier {
    MgmtgrpcServiceFileDescriptorSupplier() {}
  }

  private static final class MgmtgrpcServiceMethodDescriptorSupplier
      extends MgmtgrpcServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final String methodName;

    MgmtgrpcServiceMethodDescriptorSupplier(String methodName) {
      this.methodName = methodName;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.MethodDescriptor getMethodDescriptor() {
      return getServiceDescriptor().findMethodByName(methodName);
    }
  }

  private static volatile io.grpc.ServiceDescriptor serviceDescriptor;

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    io.grpc.ServiceDescriptor result = serviceDescriptor;
    if (result == null) {
      synchronized (MgmtgrpcServiceGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new MgmtgrpcServiceFileDescriptorSupplier())
              .addMethod(getCreateSapphireObjectMethod())
              .addMethod(getDeleteSapphireObjectMethod())
              .addMethod(getGenericMethodInvokeMethod())
              .build();
        }
      }
    }
    return result;
  }
}
