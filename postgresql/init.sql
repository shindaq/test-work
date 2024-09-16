DROP SCHEMA IF EXISTS stock_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS stock_schema;

CREATE TYPE stock_schema.operation_types AS ENUM (
    'buy',
    'sell'
);

CREATE TYPE stock_schema.requisition_status AS ENUM (
    'open',
    'closed'
);


CREATE TABLE IF NOT EXISTS stock_schema.requisitions(
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    created_at TIMESTAMPTZ DEFAULT now(),
    user_id UUID NOT NULL,
    volume INT NOT NULL CHECK(volume > 0),
    exchange_rate INT NOT NULL,
    operation_type stock_schema.operation_types NOT NULL,
    status stock_schema.requisition_status NOT NULL DEFAULT 'open'
);

CREATE INDEX requisitions_created_at_idx ON stock_schema.requisitions(created_at);

CREATE TABLE IF NOT EXISTS stock_schema.users(
    id uuid PRIMARY KEY DEFAULT gen_random_uuid(),
    name TEXT NOT NULL UNIQUE,
    usd_balance INT DEFAULT 0,
    rub_balance INT DEFAULT 0
);

CREATE TABLE IF NOT EXISTS stock_schema.deals(
    id uuid PRIMARY KEY DEFAULT gen_random_uuid(),
    created_at TIMESTAMPTZ DEFAULT now(),
    buyer_id uuid NOT NULL,
    seller_id uuid NOT NULL,
    volume INT NOT NULL,
    exchange_rate INT NOT NULL
);

CREATE INDEX deals_created_at_idx ON stock_schema.deals(created_at);